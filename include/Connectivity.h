#ifndef NANO33BLE_CONNECTIVITY_H
#define NANO33BLE_CONNECTIVITY_H

#include <ArduinoBLE.h>
#include <chrono>

#define DATE_UTC_BUFFER_SIZE 80

namespace pirids {
    class Connectivity {

    private:
        Connectivity();
        static Connectivity *instance;

        std::chrono::milliseconds utc_epoch_ms;
        bool dateSet;

        // BLE Boolean Characteristic
        BLEBooleanCharacteristic walletOut;
        
        // BLE Date UTC Characteristic
        BLEStringCharacteristic whenWalletOut, synchroDate;

    protected:
        std::string getStrDateUTC();
        void setUTCEpochMs(const std::chrono::milliseconds &utcEpochMs);

    public:
        static Connectivity *getInstance();
        void run();

    };
}

#endif //NANO33BLE_CONNECTIVITY_H
