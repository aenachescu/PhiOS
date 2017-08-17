#ifndef PhiOS_RealTimeClock
#define PhiOS_RealTimeClock

#include "include/types.h"
#include "include/errors.h"

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

uint32 RTC_init();

uint32 RTC_getDateAndTime(
    RTC *a_rtc
);

#endif
