#ifndef NANO33BLE_ANALYSIS_H
#define NANO33BLE_ANALYSIS_H

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

namespace pirids {
    class Analysis {

    private:
        Analysis(int kChannelNumber);
        static Analysis *instance;

        int predictGesture(float* output);
        bool handleOutput(int kind);

        tflite::ErrorReporter* error_reporter;
        const tflite::Model* model;
        tflite::MicroInterpreter* interpreter;
        TfLiteTensor* model_input;
        int input_length;

        // Create an area of memory to use for input, output, and intermediate arrays.
        // The size of this will depend on the model you're using, and may need to be
        // determined by experimentation.
        constexpr static int kTensorArenaSize = 60 * 1024;
        uint8_t tensor_arena[kTensorArenaSize];

        // What gestures are supported.
        constexpr static int kGestureCount = 2;
        constexpr static int kWalletGesture = 0;
        constexpr static int kNoGesture = 1;

        // These control the sensitivity of the detection algorithm. If you're seeing
        // too many false positives or not enough true positives, you can try tweaking
        // these thresholds. Often, increasing the size of the training set will give
        // more robust results though, so consider retraining if you are seeing poor
        // predictions.
        constexpr static float kDetectionThreshold = 0.8f;
        constexpr static int kPredictionHistoryLength = 5;
        constexpr static int kPredictionSuppressionDuration = 5;//25

        // State for the averaging algorithm we're using.
        float prediction_history[kGestureCount][kPredictionHistoryLength];
        int prediction_history_index;
        int prediction_suppression_count;

    public:
        static Analysis *getInstance(int kChannelNumber);
        void initModel(TfLiteTensor *model_in, int *length);
        bool run();
    };
}

#endif //NANO33BLE_ANALYSIS_H
