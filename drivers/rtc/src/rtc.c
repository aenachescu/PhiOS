#include "rtc.h"
#include "arch/x86/asm_io.h"
#include "cpu.h"
#include "kstring.h"

static RTC g_rtc;

#define CMOS_OUT_PORT    0x70
#define CMOS_IN_PORT     0x71

#define RTC_REGISTER_SECONDS    0x00
#define RTC_REGISTER_MINUTES    0x02
#define RTC_REGISTER_HOURS      0x04
#define RTC_REGISTER_DAYS       0x07
#define RTC_REGISTER_MONTHS     0x08
#define RTC_REGISTER_YEARS      0x09
#define RTC_REGISTER_CENTURIES  0x32
#define RTC_REGISTER_A          0x0A
#define RTC_REGISTER_B          0x0B

#define RTC_MODE                0x04
#define RTC_HOUR_FORMAT         0x80
#define RTC_HOUR_FLAG           0x02

#define RTC_UPDATE_FLAG         0x80

#define READ_REGISTERS                                              \
    seconds   = helper_getRegisterValue(RTC_REGISTER_SECONDS);      \
    minutes   = helper_getRegisterValue(RTC_REGISTER_MINUTES);      \
    hours     = helper_getRegisterValue(RTC_REGISTER_HOURS);        \
    days      = helper_getRegisterValue(RTC_REGISTER_DAYS);         \
    months    = helper_getRegisterValue(RTC_REGISTER_MONTHS);       \
    years     = helper_getRegisterValue(RTC_REGISTER_YEARS);        \
    centuries = helper_getRegisterValue(RTC_REGISTER_CENTURIES);

#define NOT_SAME_VALUES                 \
    g_rtc.seconds   != seconds  ||      \
    g_rtc.minutes   != minutes  ||      \
    g_rtc.hours     != hours    ||      \
    g_rtc.days      != days     ||      \
    g_rtc.months    != months   ||      \
    g_rtc.years     != years    ||      \
    g_rtc.centuries != centuries

static uint8 helper_checkIfUpdateIsInProgress()
{
    turnOffInts();

    io_outb(CMOS_OUT_PORT, RTC_REGISTER_A);
    uint8 ret = (io_inb(CMOS_IN_PORT) & RTC_UPDATE_FLAG);

    turnOnInts();

    return ret;
}

static uint8 helper_getRegisterValue(
    uint8 a_reg)
{
    turnOffInts();

    while (helper_checkIfUpdateIsInProgress() != 0);

    io_outb(CMOS_OUT_PORT, a_reg);
    uint8 ret = io_inb(CMOS_IN_PORT);

    turnOnInts();

    return ret;
}

static void helper_readDateAndTime()
{
    uint8 seconds;
    uint8 minutes;
    uint8 hours;
    uint8 days;
    uint8 months;
    uint8 years;
    uint8 centuries;

    READ_REGISTERS

    do {
        g_rtc.seconds      = seconds;
        g_rtc.minutes      = minutes;
        g_rtc.hours        = hours;
        g_rtc.days         = days;
        g_rtc.months       = months;
        g_rtc.years        = years;
        g_rtc.centuries    = centuries;

        READ_REGISTERS
    } while(NOT_SAME_VALUES);

    uint8 registerB = helper_getRegisterValue(RTC_REGISTER_B);

    // check if is BCD mode
    if ((registerB & RTC_MODE) == 0) {
        g_rtc.hours      = (g_rtc.hours       & 0x0F) + (g_rtc.hours       / 16) * 10;
        g_rtc.seconds    = (g_rtc.seconds     & 0x0F) + (g_rtc.seconds     / 16) * 10;
        g_rtc.minutes    = (g_rtc.minutes     & 0x0F) + (g_rtc.minutes     / 16) * 10;
        g_rtc.days       = (g_rtc.days        & 0x0F) + (g_rtc.days        / 16) * 10;
        g_rtc.months     = (g_rtc.months      & 0x0F) + (g_rtc.months      / 16) * 10;
        g_rtc.years      = (g_rtc.years       & 0x0F) + (g_rtc.years       / 16) * 10;
        g_rtc.centuries  = (g_rtc.centuries   & 0x0F) + (g_rtc.centuries   / 16) * 10;
    }

    // check if is in 12 hour format
    if ((registerB & RTC_HOUR_FLAG) == 0 && (g_rtc.hours & RTC_HOUR_FORMAT) != 0) {
        g_rtc.hours = ((g_rtc.hours & 0x7F) + 12) % 24;
    }
}

size_t RTC_init()
{
    kmemset(&g_rtc, 0, sizeof(RTC));

    return ERROR_SUCCESS;
}

size_t RTC_getDateAndTime(
    RTC *a_rtc)
{
    if (a_rtc == NULL) {
        return ERROR_NULL_POINTER;
    }

    helper_readDateAndTime();

    a_rtc->seconds      = g_rtc.seconds;
    a_rtc->minutes      = g_rtc.minutes;
    a_rtc->hours        = g_rtc.hours;
    a_rtc->days         = g_rtc.days;
    a_rtc->months       = g_rtc.months;
    a_rtc->years        = g_rtc.years;
    a_rtc->centuries    = g_rtc.centuries;

    return ERROR_SUCCESS;
}
