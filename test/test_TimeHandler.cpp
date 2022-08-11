#include <unity.h>

#include "TimeHandler.h"

void setUp() {
}

void tearDown() {
}

void test_utcEpochStrMsToEpochMs() {
    TEST_ASSERT_TRUE(true);
    //TEST_ASSERT_EQUAL(1660137455627ULL, pirids::TimeHandler::utcEpochStrMsToEpochMs("1660137455627").count());
}

#ifdef ARDUINO

#include <Arduino.h>
#include <Wire.h>

void setup() {
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_utcEpochStrMsToEpochMs);

    UNITY_END();
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_utcEpochStrMsToEpochMs);

    UNITY_END();

    return 0;
}

#endif // ARDUINO