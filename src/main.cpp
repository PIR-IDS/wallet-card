#include <Arduino.h>

#include "Connectivity.h"
#include "Sensor.h"
#include "Analysis.h"

pirids::Connectivity *c;
pirids::Sensor *s;
pirids::Analysis *a;

void setup()
{
    c = pirids::Connectivity::getInstance();
    a = pirids::Analysis::getInstance(CHANNEL_NUMBER);
    s = pirids::Sensor::getInstance();
}

void loop()
{
    TfLiteTensor *model_input = nullptr;
    int input_length;

    //c->run();
    a->initModel(model_input, &input_length);
    // Attempt to read new data from the accelerometer.
    bool got_data = s->readAccelerometer(model_input->data.f, input_length);
    // If there was no new data, wait until next time.
    if(!got_data) return;

    a->run();
    Serial.print(".");
}
