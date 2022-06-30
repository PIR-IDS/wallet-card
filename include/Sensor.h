#ifndef NANO33BLE_SENSOR_H
#define NANO33BLE_SENSOR_H

#include <Arduino_LSM9DS1.h>

#define ACCEL_SAMPLES_PER_SECOND 25
#define CHANNEL_NUMBER 3

namespace pirids {
    class Sensor {

    private:
        Sensor();
        static Sensor *instance;

        int setupAccelerometer();

        // A buffer holding the last 300 sets of 3-channel values
        float save_data[600];
        // Most recent position in the save_data buffer
        int begin_index;
        // True if there is not yet enough data to run inference
        bool pending_initial_data;
        // How often we should save a measurement during downsampling
        int sample_every_n;
        // The number of measurements since we last saved one
        int sample_skip_counter;

    public:
        static Sensor *getInstance();
        bool readAccelerometer(float* input, int length);
    };
}

#endif //NANO33BLE_SENSOR_H
