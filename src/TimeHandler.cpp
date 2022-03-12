#include "TimeHandler.h"

#include <chrono>
#include <ctime>
#include <string>

std::chrono::milliseconds pirids::TimeHandler::utc_epoch_ms = std::chrono::milliseconds::zero(); // Will be filled with true value from the BLE device when connected

std::string pirids::TimeHandler::getStrDateUTC() {
    auto now = std::chrono::system_clock::now() + utc_epoch_ms;
    auto now_c = std::chrono::system_clock::to_time_t(now);

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
    utc_epoch_ms = utcEpochMs;
}

std::chrono::milliseconds pirids::TimeHandler::utcStrToEpochMs(const std::string &utc) {
    struct tm tm = {0};
    // FIXME
    //strptime(utc.c_str(), "%Y-%m-%dT%H:%M:%S.000Z", &tm);
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(std::mktime(&tm)).time_since_epoch());
}