#ifndef NANO33BLE_CONNECTIVITY_H
#define NANO33BLE_CONNECTIVITY_H

#include <ArduinoBLE.h>
#include "Analysis.h"
#include "Sensor.h"

namespace pirids {
    class Connectivity {

    private:
        Connectivity();
        static Connectivity *instance;

        bool dateSet;

        // BLE Boolean Characteristic
        BLEBooleanCharacteristic walletOut;
        
        // BLE Date UTC Characteristic
        BLEStringCharacteristic whenWalletOut, synchroDate;

    public:
        static Connectivity *getInstance();
        void run(pirids::Analysis *a, pirids::Sensor *s);

    };
}

#endif //NANO33BLE_CONNECTIVITY_H
