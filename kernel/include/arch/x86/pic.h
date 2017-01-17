#ifndef PhiOS_ProgrammableInterruptController
#define PhiOS_ProgrammableInterruptController

#define PIC1_COMMAND                0x20
#define PIC1_DATA                   0x21
#define PIC2_COMMAND                0xA0
#define PIC2_DATA                   0xA1

size_t PIC_init();
size_t PIC_setMask(uint8 a_irqLine);
size_t PIC_clearMask(uint8 a_irqLine);
size_t PIC_maskUnusedIRQs();

#endif
