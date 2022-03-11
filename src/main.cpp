#include <Arduino.h>

#include "Connectivity.h"
#include "IMU.h"
#include "Analysis.h"

pirids::Connectivity *c;

void setup()
{
    c = pirids::Connectivity::getInstance();
}

void loop()
{
    c->run();
}
