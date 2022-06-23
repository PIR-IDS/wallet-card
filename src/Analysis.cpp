#include "Analysis.h"
#include "../res/wallet_model_data.cpp" // weird include but it works...

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

pirids::Analysis *pirids::Analysis::instance = nullptr;

pirids::Analysis *pirids::Analysis::getInstance(int kChannelNumber) {
    if(instance == nullptr) {
        instance = new pirids::Analysis(kChannelNumber);
    }
    return instance;
}

pirids::Analysis::Analysis(int kChannelNumber):
        error_reporter(nullptr),
        model(nullptr),
        interpreter(nullptr),
        model_input(nullptr),
        prediction_history{},
        prediction_history_index(0),
        prediction_suppression_count(0)
{
    // Set up logging.
    static tflite::MicroErrorReporter micro_error_reporter;  // NOLINT
    error_reporter = &micro_error_reporter;

    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    model = tflite::GetModel(wallet_model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.",
                             model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    // Pull in only the operation implementations we need.
    // This relies on a complete list of all the ops needed by this graph.
    // An easier approach is to just use the AllOpsResolver, but this will
    // incur some penalty in code space for op implementations that are not
    // needed by this graph.
    static tflite::MicroMutableOpResolver<5> micro_op_resolver;  // NOLINT
    micro_op_resolver.AddConv2D();
    micro_op_resolver.AddFullyConnected();
    micro_op_resolver.AddMaxPool2D();
    micro_op_resolver.AddSoftmax();
    micro_op_resolver.AddReshape();

    // Build an interpreter to run the model with.
    static tflite::MicroInterpreter static_interpreter(
            model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    interpreter->AllocateTensors();

    // Obtain pointer to the model's input tensor.
    model_input = interpreter->input(0);
    if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
        (model_input->dims->data[1] != dataAmout) ||
        (model_input->dims->data[2] != kChannelNumber) ||
        (model_input->type != kTfLiteFloat32)) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Bad input tensor parameters in model");
        return;
    }

    input_length = model_input->bytes / sizeof(float);
}

void pirids::Analysis::initModel(TfLiteTensor **model_in, int *length) {
    *model_in = model_input;
    *length = input_length;
}


bool pirids::Analysis::run() {
    // Run inference, and report any error.
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on current index\n");
        return false;
    }

    // Analyze the results to obtain a prediction
    int gesture_index = predictGesture(interpreter->output(0)->data.f);
    // Produce an output
    return handleOutput(gesture_index);
}

/**
 * Return the result of the last prediction
 * 0: wallet("W"), 1: unknown
 */
int pirids::Analysis::predictGesture(float* output) {
    // Record the latest predictions in our rolling history buffer.
    for (int i = 0; i < kGestureCount; ++i) {
        prediction_history[i][prediction_history_index] = output[i];
    }
    // Figure out which slot to put the next predictions into.
    ++prediction_history_index;
    if (prediction_history_index >= kPredictionHistoryLength) {
        prediction_history_index = 0;
    }

    // Average the last n predictions for each gesture, and find which has the
    // highest score.
    int max_predict_index = -1;
    float max_predict_score = 0.0f;
    for (int i = 0; i < kGestureCount; i++) {
        float prediction_sum = 0.0f;
        for (int j = 0; j < kPredictionHistoryLength; ++j) {
            prediction_sum += prediction_history[i][j];
        }
        const float prediction_average = prediction_sum / kPredictionHistoryLength;
        if ((max_predict_index == -1) || (prediction_average > max_predict_score)) {
            max_predict_index = i;
            max_predict_score = prediction_average;
        }
    }

    // If there's been a recent prediction, don't trigger a new one too soon.
    if (prediction_suppression_count > 0) {
        --prediction_suppression_count;
    }
    // If we're predicting no gesture, or the average score is too low, or there's
    // been a gesture recognised too recently, return no gesture.
    if ((max_predict_index == kNoGesture) ||
        (max_predict_score < kDetectionThreshold) ||
        (prediction_suppression_count > 0)) {
        return kNoGesture;
    } else {
        // Reset the suppression counter so we don't come up with another prediction
        // too soon.
        prediction_suppression_count = kPredictionSuppressionDuration;
        return max_predict_index;
    }
}

bool pirids::Analysis::handleOutput(int kind) {
    if (kind == kWalletGesture) {
        TF_LITE_REPORT_ERROR(
                error_reporter,
                "WALLET OUT:\n\r*         *         *\n\r *       * *       "
                "*\n\r  *     *   *     *\n\r   *   *     *   *\n\r    * *       "
                "* *\n\r     *         *\n\r");
        return true;
    }
    return false;
}