#include <Arduino.h>

#include "Connectivity.h"
#include "IMU.h"
#include "Analysis.h"

void setup()
{
    Serial.begin(9600);
    Serial.println("set up");
}

void loop()
{
    Serial.println("loop...");
    delay(1000);
}
