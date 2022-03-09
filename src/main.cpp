#include <Arduino.h>

#include "Connectivity.h"
#include "IMU.h"
#include "Analysis.h"

// FIXME: use Connectivity instead of copying the code into this file...

//pirids::Connectivity c;

// BLE Boolean Characteristic
BLEBooleanCharacteristic walletOut("2AE2",  // standard 16-bit Boolean characteristic UUID
                                   BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
// BLE Date UTC Characteristic
BLEStringCharacteristic whenWalletOut("2AED",  // standard 16-bit Date UTC characteristic UUID
                                      BLERead | BLENotify, // remote clients will be able to get notifications if this characteristic changes
                                      80);
void setup()
{
    Serial.begin(9600);
    Serial.println("set up");

    //c = pirids::Connectivity();
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


    BLE.setAdvertisedService(idsService); // add the service UUID

    idsService.addCharacteristic(walletOut); // add the boolean characteristic
    idsService.addCharacteristic(whenWalletOut); // add the datetime characteristic

    BLE.addService(idsService); // Add the alert service

    walletOut.writeValue(false); // set initial value for walletOut
    auto now = std::chrono::system_clock::now() + std::chrono::milliseconds::zero();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::chrono::milliseconds epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    char dateUTCBuffer[80];
    strftime(dateUTCBuffer, sizeof(dateUTCBuffer), "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&now_c));
    std::string str(dateUTCBuffer);
    whenWalletOut.writeValue(str.c_str()); // set initial value for UTC Date

    // start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop()
{
    // wait for a BLE central
    BLEDevice central = BLE.central();

    // if a central is connected to the peripheral:
    if (central) {
        Serial.print("Connected to central: ");
        // print the central's BT address:
        Serial.println(central.address());
        // turn on the LED to indicate the connection:
        digitalWrite(LED_BUILTIN, HIGH);
    }

    while (central.connected()) {
        auto now = std::chrono::system_clock::now() + std::chrono::milliseconds::zero();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::chrono::milliseconds epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

        char dateUTCBuffer[80];
        strftime(dateUTCBuffer, sizeof(dateUTCBuffer), "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&now_c));
        std::string str(dateUTCBuffer);
        whenWalletOut.writeValue(str.c_str()); // set initial value for UTC Date
        delay(1000);
        if(!central.connected()) {
            // when the central disconnects, turn off the LED:
            digitalWrite(LED_BUILTIN, LOW);
            Serial.print("Disconnected from central: ");
            Serial.println(central.address());
            break;
        }
    }
}
