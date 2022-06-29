#include "Connectivity.h"
#include "TimeHandler.h"
#include "Analysis.h"
#include "Sensor.h"

#include <ArduinoBLE.h>

pirids::Connectivity *pirids::Connectivity::instance = nullptr;

pirids::Connectivity *pirids::Connectivity::getInstance() {
    if(instance == nullptr) {
        instance = new pirids::Connectivity();
    }
    return instance;
}

pirids::Connectivity::Connectivity():
    dateSet(false),
    walletOut("2AE2",               // standard 16-bit Boolean characteristic UUID
            BLERead | BLENotify),   // remote clients will be able to get notifications if this characteristic changes
    whenWalletOut("2AED",           // standard 16-bit Date UTC characteristic UUID
            BLERead | BLENotify,    // remote clients will be able to get notifications if this characteristic changes
            DATE_UTC_BUFFER_SIZE), 
    synchroDate("2A2B",             // standard 16-bit Current Time characteristic UUID
            BLEWrite | BLENotify,   // remote clients will be able to change this characteristic
            DATE_UTC_BUFFER_SIZE)
{
    /* ------ INITIALIZE BLE ------ */

    if(!BLE.begin()) {
        Serial.println("starting BLE failed!");
    }

    BLE.setDeviceName("PIR-IDS Card");
    BLE.setLocalName("PIR-IDS WALLET CARD");

    // Generic Access Control (the closest one to an IDS)
    // see: https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf
    BLE.setAppearance(0x0700);

    /* ------ UTC RETRIEVER SERVICE ------ */

    // BLE Current Time 16-bit UUID Service
    // see: https://btprodspecificationrefs.blob.core.windows.net/assigned-values/16-bit%20UUID%20Numbers%20Document.pdf
    BLEService idsServiceDate("1805");
    
    idsServiceDate.addCharacteristic(synchroDate); // datetime

    // add the IDS Date Retriever service
    BLE.addService(idsServiceDate);
    
    /* ------ IDS IMU SERVICE ------ */

    // BLE IDS IMU custom 128-bit UUID Service
    // see: https://btprodspecificationrefs.blob.core.windows.net/assigned-values/16-bit%20UUID%20Numbers%20Document.pdf
    BLEService idsServiceWallet("D70C4BB1-98E4-4EBF-9EA5-F9898690D428");
    BLE.setAdvertisedService(idsServiceWallet); // add the service UUID

    // add the characteristics
    idsServiceWallet.addCharacteristic(walletOut); // boolean
    idsServiceWallet.addCharacteristic(whenWalletOut); // datetime
    
    // add the IDS IMU service
    BLE.addService(idsServiceWallet);

    walletOut.writeValue(false); // set initial value for walletOut

    whenWalletOut.writeValue(TimeHandler::getStrDateUTC().c_str()); // set initial value for UTC Date

    /* ------ ADVERTISE ------ */

    // start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
}

void pirids::Connectivity::run(pirids::Analysis *a, pirids::Sensor *s)
{
    BLEDevice central = BLE.central();

    if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());
        digitalWrite(LED_BUILTIN, HIGH);

        if(dateSet) {
            Serial.println("IDS ready.");
            Serial.println("IDS ON.");
        }
    }

    while (central.connected()) {
        if(!dateSet) {
            Serial.println("Waiting for Date UTC...");
            char buf[DATE_UTC_BUFFER_SIZE] = "\0";
            do {
                //delay(100);
                synchroDate.read();
                if(synchroDate.valueLength() <= 0) continue;
                synchroDate.readValue(buf, DATE_UTC_BUFFER_SIZE);
            } while(std::strcmp("", buf) == 0 && central.connected());
            std::string buf_str(buf);
            if(std::all_of(buf_str.begin(), buf_str.end(), ::isdigit) && central.connected()) {
                TimeHandler::setUTCEpochMs(TimeHandler::utcEpochStrMsToEpochMs(buf_str));
                dateSet = true;
                Serial.println("IDS ready.");
                Serial.println("IDS ON.");
            } else {
                Serial.println("Initialization error, retrying...");
                if(central.connected()) {
                    continue;
                }
            }
        }

        if(central.connected()) {
            bool walletIsOut;

            /* ------ IDS PROCESSING ----------- */

            TfLiteTensor *model_input = nullptr;
            int input_length;

            a->initModel(&model_input, &input_length);
            // Attempt to read new data from the accelerometer.
            bool got_data = s->readAccelerometer(model_input->data.f, input_length);
            // If there was no new data, wait until next time.
            if (!got_data) continue;

            walletIsOut = a->run();

            /* -------------------------------- */

            if (walletIsOut) {
                whenWalletOut.writeValue(TimeHandler::getStrDateUTC().c_str());
                walletOut.writeValue(true);
            }
        }

        if(!central.connected()) {
            Serial.print("Disconnected from central: ");
            Serial.println(central.address());
            digitalWrite(LED_BUILTIN, LOW);
            if(dateSet) {
                Serial.println("IDS OFF.");
            }
            break;
        }
    }
}