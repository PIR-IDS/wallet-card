#ifndef NANO33BLE_TIMEHANDLER_H
#define NANO33BLE_TIMEHANDLER_H

#include <chrono>
#include <string>

#define DATE_UTC_BUFFER_SIZE 80

namespace pirids {
    class TimeHandler {
    private:
        static std::chrono::milliseconds utc_epoch_ms;
    public:
        static std::string getStrDateUTC();
        static void setUTCEpochMs(const std::chrono::milliseconds &utcEpochMs);
        static std::chrono::milliseconds utcStrToEpochMs(const std::string &utc);
    };
}


#endif //NANO33BLE_TIMEHANDLER_H
