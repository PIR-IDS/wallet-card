#ifndef NANO33BLE_TIMEHANDLER_H
#define NANO33BLE_TIMEHANDLER_H

#include <chrono>
#include <string>

#define DATE_UTC_BUFFER_SIZE 80

#define DATE_OFFSET_SEC_70_YEARS 2208988805ULL
#define DATE_OFFSET_SEC_1_DAY 86400ULL
#define DATE_OFFSET_SEC_C (DATE_OFFSET_SEC_70_YEARS + DATE_OFFSET_SEC_1_DAY)

namespace pirids {
    class TimeHandler {
    private:
        using chrono_clock = std::chrono::system_clock;
        static std::chrono::milliseconds utc_epoch_ms;
    public:
        static std::string getStrDateUTC();
        static void setUTCEpochMs(const std::chrono::milliseconds &utcEpochMs);
        static std::chrono::milliseconds utcEpochStrMsToEpochMs(const std::string &utc);
    };
}


#endif //NANO33BLE_TIMEHANDLER_H
