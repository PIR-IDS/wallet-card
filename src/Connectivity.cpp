#include "Connectivity.h"

#include <ArduinoBLE.h>

#include <chrono>
#include <ctime>

#define DATE_UTC_BUFFER_SIZE 80

std::chrono::milliseconds pirids::Connectivity::real_epoch_ms = std::chrono::milliseconds::zero(); // Will be filled with true value from the BLE device when connected

pirids::Connectivity::Connectivity() {
    if(!BLE.begin()) {
        Serial.println("starting BLE failed!");
    }

    BLE.setDeviceName("PIR-IDS Card");
    BLE.setLocalName("PIR-IDS");

    // Generic Access Control (the closest one to an IDS)
    // see: https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf
    BLE.setAppearance(0x0700);

    // BLE IDS IMU custom 128-bit UUID Service
    // see: https://btprodspecificationrefs.blob.core.windows.net/assigned-values/16-bit%20UUID%20Numbers%20Document.pdf
    BLEService idsService("D70C4BB1-98E4-4EBF-9EA5-F9898690D428");

    // BLE Boolean Characteristic
    BLEBooleanCharacteristic walletOut("2AE2",  // standard 16-bit Boolean characteristic UUID
       BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
    // BLE Date UTC Characteristic
    BLEStringCharacteristic whenWalletOut("2AED",  // standard 16-bit Date UTC characteristic UUID
        BLERead | BLENotify, // remote clients will be able to get notifications if this characteristic changes
    DATE_UTC_BUFFER_SIZE);

    BLE.setAdvertisedService(idsService); // add the service UUID

    idsService.addCharacteristic(walletOut); // add the boolean characteristic
    idsService.addCharacteristic(whenWalletOut); // add the datetime characteristic

    BLE.addService(idsService); // Add the alert service

    walletOut.writeValue(false); // set initial value for walletOut

    whenWalletOut.writeValue(getStrDateUTC().c_str()); // set initial value for UTC Date

    // start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
}

std::string pirids::Connectivity::getStrDateUTC() {
    auto now = std::chrono::system_clock::now() + Connectivity::real_epoch_ms;
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::chrono::milliseconds epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    char dateUTCBuffer[DATE_UTC_BUFFER_SIZE];
    strftime(dateUTCBuffer, sizeof(dateUTCBuffer), "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&now_c));
    std::string str(dateUTCBuffer);

    return str;
}

/**
 * Set the real epoch value when called, retrieved when a BLE device is connected
 * @param realEpochMs
 */
void pirids::Connectivity::setRealEpochMs(const std::chrono::milliseconds &realEpochMs) {
    Connectivity::real_epoch_ms = realEpochMs;
}
