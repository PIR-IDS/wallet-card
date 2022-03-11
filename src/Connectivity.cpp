#include "Connectivity.h"

#include <ArduinoBLE.h>

#include <chrono>
#include <ctime>

pirids::Connectivity *pirids::Connectivity::instance = nullptr;

pirids::Connectivity *pirids::Connectivity::getInstance() {
    if(instance == nullptr) {
        instance = new pirids::Connectivity();
    }
    return instance;
}

pirids::Connectivity::Connectivity():
    utc_epoch_ms(std::chrono::milliseconds::zero()), // Will be filled with true value from the BLE device when connected
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
    BLE.setLocalName("PIR-IDS");

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

    whenWalletOut.writeValue(getStrDateUTC().c_str()); // set initial value for UTC Date

    /* ------ ADVERTISE ------ */

    // start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
}

void pirids::Connectivity::run()
{
    BLEDevice central = BLE.central();

    if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());
        digitalWrite(LED_BUILTIN, HIGH);
    }

    while (central.connected()) {
        if(!dateSet) {
            char buf[DATE_UTC_BUFFER_SIZE] = "\0";
            synchroDate.readValue(buf,DATE_UTC_BUFFER_SIZE);
            Serial.println(buf);
        }

        whenWalletOut.writeValue(getStrDateUTC().c_str());
        delay(1000);

        if(!central.connected()) {
            Serial.print("Disconnected from central: ");
            Serial.println(central.address());
            digitalWrite(LED_BUILTIN, LOW);
            break;
        }
    }
}

std::string pirids::Connectivity::getStrDateUTC() {
    auto now = std::chrono::system_clock::now() + utc_epoch_ms;
    auto now_c = std::chrono::system_clock::to_time_t(now);

    char dateUTCBuffer[DATE_UTC_BUFFER_SIZE];
    strftime(dateUTCBuffer, sizeof(dateUTCBuffer), "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&now_c));
    std::string str(dateUTCBuffer);
    return str;
}

/**
 * Set the real epoch value when called, retrieved when a BLE device is connected
 * @param realEpochMs
 */
void pirids::Connectivity::setUTCEpochMs(const std::chrono::milliseconds &utcEpochMs) {
    utc_epoch_ms = utcEpochMs;
}