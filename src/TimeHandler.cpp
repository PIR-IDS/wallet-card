#include "TimeHandler.h"

#include <chrono>
#include <ctime>
#include <string>

std::chrono::milliseconds pirids::TimeHandler::utc_epoch_ms = std::chrono::milliseconds::zero(); // Will be filled with true value from the BLE device when connected

/**
 * Get the UTC date in its standard text format
 * @return
 */
std::string pirids::TimeHandler::getStrDateUTC() {
    auto now = chrono_clock::now() + utc_epoch_ms;
    auto now_c = chrono_clock::to_time_t(now);

    char dateUTCBuffer[DATE_UTC_BUFFER_SIZE];
    std::strftime(dateUTCBuffer, sizeof(dateUTCBuffer), "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&now_c));
    std::string str(dateUTCBuffer);
    return str;
}

/**
 * Set the real epoch value when called, retrieved when a BLE device is connected
 * @param utcEpochMs
 */
void pirids::TimeHandler::setUTCEpochMs(const std::chrono::milliseconds &utcEpochMs) {
    utc_epoch_ms = utcEpochMs - std::chrono::duration_cast<std::chrono::milliseconds>(chrono_clock::now().time_since_epoch());
}

/**
 * Convert the string UTC Epoch milliseconds in chrono milliseconds
 * We can't use strptime with this MCU architecture so we need to retrieve the date in milliseconds instead of the UTC standard text format
 * @param utc UTC Epoch in milliseconds in std::string format
 * @return
 */
std::chrono::milliseconds pirids::TimeHandler::utcEpochStrMsToEpochMs(const std::string &utc) {
    struct tm tm = { .tm_sec = std::stoi(utc) / 1000 }; // no milliseconds precision so we use seconds
    return std::chrono::duration_cast<std::chrono::milliseconds>(chrono_clock::from_time_t(std::mktime(&tm)).time_since_epoch());
}