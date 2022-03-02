#include <Arduino.h>

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

void setup()
{
    Serial.begin(9600);
    Serial.println("set up");

    // set up TF logging
    tflite::MicroErrorReporter micro_error_reporter;
    tflite::ErrorReporter* error_reporter = &micro_error_reporter;
}

void loop()
{
    Serial.println("loop...");
    delay(1000);
}
