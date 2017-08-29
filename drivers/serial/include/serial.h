#ifndef PhiOS_SerialDriver
#define PhiOS_SerialDriver

#include "include/types.h"
#include "include/errors.h"

#define SERIAL_PORT_A ((uint16) 0x3F8)
#define SERIAL_PORT_B ((uint16) 0x2F8)
#define SERIAL_PORT_C ((uint16) 0x3E8)
#define SERIAL_PORT_D ((uint16) 0x2E8)

uint32 serial_init();

uint32 serial_writeString(
    const char *a_str,
    uint16 a_port
);

#endif
