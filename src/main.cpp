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
    c->run(a, s);
}
