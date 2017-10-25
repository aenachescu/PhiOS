#include "drivers/serial/include/serial.h"

#include "kernel/include/arch/x86/asm_io.h"
#include "kernel/include/arch/x86/idt.h"

static bool g_isInit = false;

static void serial_com1Handler(
    __attribute__((unused)) CpuState *a_state
);

static void serial_com2Handler(
    __attribute__((unused)) CpuState *a_state
);

static void serial_enable(
    uint16 a_port)
{
	io_outb(a_port + 1, 0x00); /* Disable interrupts */
	io_outb(a_port + 3, 0x80); /* Enable divisor mode */
	io_outb(a_port + 0, 0x01); /* Div Low:  01 Set the port to 115200 bps */
	io_outb(a_port + 1, 0x00); /* Div High: 00 */
	io_outb(a_port + 3, 0x03); /* Disable divisor mode, set parity */
	io_outb(a_port + 2, 0xC7); /* Enable FIFO and clear */
	io_outb(a_port + 4, 0x0B); /* Enable interrupts */
	io_outb(a_port + 1, 0x01); /* Enable interrupts */

    if (a_port == SERIAL_PORT_A) {
        IDT_registerHandler(IRQ4, serial_com1Handler);
    }

    if (a_port == SERIAL_PORT_B) {
        IDT_registerHandler(IRQ3, serial_com2Handler);
    }
}

static bool serial_received(uint16 a_port)
{
    if ((io_inb(a_port + 5) & 1) == 0) {
        return false;
    }

    return true;
}

static uint8 serial_read(uint16 a_port)
{
	while (serial_received(a_port) == false) {
        pauseCpu();
    }

	return io_inb(a_port);
}

static bool serial_isEmpty(uint16 a_port)
{
    if ((io_inb(a_port + 5) & 0x20) == 0) {
        return false;
    }

    return true;
}

static void serial_write(uint16 a_port, uint8 a_data)
{
	while (serial_isEmpty(a_port) == false) {
        pauseCpu();
    }

	io_outb(a_port, a_data);
}

static void serial_com1Handler(
    __attribute__((unused)) CpuState *a_state)
{
    uint16 port = 0;
    uint8 c = 0;

    if ((io_inb(SERIAL_PORT_A + 1) & 0x01) != 0) {
        port = SERIAL_PORT_A;
    } else {
        port = SERIAL_PORT_C;
    }

    c = serial_read(port);
    kprintf("Data from port[%x] = %c\n", (uint32)port, (char)c);
}

static void serial_com2Handler(
    __attribute__((unused)) CpuState *a_state)
{
    uint16 port = 0;
    uint8 c = 0;

    if ((io_inb(SERIAL_PORT_B + 1) & 0x01) != 0) {
        port = SERIAL_PORT_B;
    } else {
        port = SERIAL_PORT_D;
    }

    c = serial_read(port);
    kprintf("Data from port[%x] = %c\n", (uint32)port, (char)c);
}

uint32 serial_init()
{
    if (g_isInit == true) {
        return ERROR_ALREADY_INITIALIZED;
    }

    serial_enable(SERIAL_PORT_A);
    serial_enable(SERIAL_PORT_B);
    serial_enable(SERIAL_PORT_C);
    serial_enable(SERIAL_PORT_D);
    
    g_isInit = true;

    return ERROR_SUCCESS;
}

uint32 serial_writeString(
    const char *a_str,
    uint16 a_port)
{
    if (a_str == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (g_isInit == false) {
        return ERROR_UNINITIALIZED;
    }

    switch (a_port) {
        case SERIAL_PORT_A:
        case SERIAL_PORT_B:
        case SERIAL_PORT_C:
        case SERIAL_PORT_D:
            break;

        default:
            return ERROR_INVALID_PORT;
    }

    while (a_str[0] != '\0') {
        serial_write(SERIAL_PORT_A, (uint8)a_str[0]);
        a_str++;
    }

    return ERROR_SUCCESS;
}

uint32 serial_writeStringDefault(
    const char *a_str)
{
    return serial_writeString(a_str, SERIAL_PORT_A);
}
