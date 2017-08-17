#ifndef PhiOS_RealTimeClock
#define PhiOS_RealTimeClock

#include "types.h"
#include "errors.h"

typedef struct rtc
{
    uint8 seconds;
    uint8 minutes;
    uint8 hours;
    uint8 days;
    uint8 months;
    uint8 years;
    uint8 centuries;
} RTC;

size_t RTC_init();

size_t RTC_getDateAndTime(
    RTC *a_rtc
);

#endif
