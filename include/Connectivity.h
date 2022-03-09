#ifndef NANO33BLE_CONNECTIVITY_H
#define NANO33BLE_CONNECTIVITY_H

#include <ArduinoBLE.h>
#include <chrono>

namespace pirids {
    class Connectivity {
    public:
        Connectivity();

    protected:
        static std::string getStrDateUTC();
        static void setRealEpochMs(const std::chrono::milliseconds &realEpochMs);

    private:
        static std::chrono::milliseconds real_epoch_ms;
    };
}

#endif //NANO33BLE_CONNECTIVITY_H
