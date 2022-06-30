#include "Sensor.h"

#include <Arduino_LSM9DS1.h>

pirids::Sensor *pirids::Sensor::instance = nullptr;

pirids::Sensor *pirids::Sensor::getInstance() {
    if(instance == nullptr) {
        instance = new pirids::Sensor();
    }
    return instance;
}

pirids::Sensor::Sensor():
        save_data{0.0},
        begin_index(0),
        pending_initial_data(true),
        sample_skip_counter(-1)
{
    setupAccelerometer();
}

int pirids::Sensor::setupAccelerometer() {
    // Switch on the IMU
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU");
        return -1;
    }

    // Make sure we are pulling measurements into a FIFO.
    IMU.setContinuousMode();

    // Determine how many measurements to keep in order to
    // meet ACCEL_SAMPLES_PER_SECOND
    float sample_rate = IMU.accelerationSampleRate();
    sample_every_n = static_cast<int>(roundf(sample_rate / ACCEL_SAMPLES_PER_SECOND));

    Serial.println("IMU started");

    return 0;
}

bool pirids::Sensor::readAccelerometer(float* input, int length) {
    // Keep track of whether we stored any new data
    bool new_data = false;
    // Loop through new samples and add to buffer
    while (IMU.accelerationAvailable()) {
        float x, y, z;

        // Read each sample, removing it from the device's FIFO buffer
        if (!IMU.readAcceleration(x, y, z)) {
            Serial.println("Failed to read data");
            break;
        }

        // Throw away this sample unless it's the nth
        if (sample_skip_counter != sample_every_n) {
            sample_skip_counter += 1;
            continue;
        }

        // Write samples to our buffer, converting to milli-Gs and rotating the axis
        // order for compatibility with model (sensor orientation is different on
        // Arduino Nano BLE Sense compared with SparkFun Edge).
        const float norm_x = -z;
        const float norm_y = y;
        const float norm_z = x;
        save_data[begin_index++] = norm_x * 1000;
        save_data[begin_index++] = norm_y * 1000;
        save_data[begin_index++] = norm_z * 1000;
        // Since we took a sample, reset the skip counter
        sample_skip_counter = 1;

        // If we reached the end of the circle buffer, reset
        if (begin_index >= 600) {
            begin_index = 0;
        }

        new_data = true;
    }

    // Skip this round if data is not ready yet
    if (!new_data) {
        return false;
    }

    // Check if we are ready for prediction or still pending more initial data
    if (pending_initial_data && begin_index >= 200) {
        pending_initial_data = false;
    }

    // Return if we don't have enough data
    if (pending_initial_data) {
        return false;
    }

    // Copy the requested number of bytes to the provided input tensor
    for (int i = 0 ; i < length ; ++i) {
        int ring_array_index = begin_index + i - length;
        if (ring_array_index < 0) {
            ring_array_index += 600;
        }
        input[i] = save_data[ring_array_index];
    }

    return true;
}