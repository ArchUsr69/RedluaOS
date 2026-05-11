#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <broadcom.h>

#ifdef BCM2712

#else

// These registers actually control more than mini UART
#define AUX_BASE (PERIPHERAL_BASE + 0x215000)
#define AUX_ENABLE ((MMIO_8)(AUX_BASE + 0x04))

#define AUX_UART_IO ((MMIO_8)(AUX_BASE + 0x40))
#define AUX_UART_INTERUPT ((MMIO_8)(AUX_BASE + 0x44))
#define AUX_UART_INTERUPT_ID ((MMIO_8)(AUX_BASE + 0x48))
#define AUX_UART_LINE_CONTROL ((MMIO_8)(AUX_BASE + 0x4C))
#define AUX_UART_MODEM_CONTROL ((MMIO_8)(AUX_BASE + 0x50))
#define AUX_UART_LINE_STATUS ((MMIO_8)(AUX_BASE + 0x54))
#define AUX_UART_EXTRA_CONTROL ((MMIO_8)(AUX_BASE + 0x60))
#define AUX_UART_EXTRA_STATUS ((MMIO_32)(AUX_BASE + 0x64))
#define AUX_UART_BAUD ((MMIO_16)(AUX_BASE + 0x68))

#define TX_FIFO_FULL 0x20
#define RX_FIFO_EMPTY HIGH

/*
-> Right now this is dependand on the clock speed; it might work, maybe not.
-> it MUST have clock speed ARM = 250 MHz; You can set it via the mailbox;
-> it calculates for baudrate 9600; until i get a math library, i can't have divisions with variable numbers;
   that's why i must use a constant;

-> I'll document it better some other time;
*/

void BCMuartInit() {
    *AUX_ENABLE = HIGH;
    *AUX_UART_LINE_CONTROL = 0x81;
    *AUX_UART_BAUD = 3250;
    *AUX_UART_LINE_CONTROL = 0x01;
    
    BCMgpioSetFunction(14, ALTERNATIVE5);
    BCMgpioSetFunction(15, ALTERNATIVE5);

    BCMgpioDisablePullup(14);
    BCMgpioDisablePullup(15);
}

// ------------------------ //

/*
-> writes to the outside worl actually \0; and it also doesn't translate \n to \r;
-> are blocking;
*/

void BCMuartWriteByte(char byte) {
    while ((*AUX_UART_LINE_STATUS & TX_FIFO_FULL) == 0) {}
    *AUX_UART_IO = byte;
}

void BCMuartWriteText(string string) {
    for (size_t index = 0;index < string.length; index++) {
        BCMuartWriteByte(string.text[index]);
    }
}

// ------------------------ //

/*
-> has a blocking and non blocking (NO INTERUPT) read function;
-> the non blocking just returns 0 if no input is present in the FIFO;
-> doesn't translate \r to \n;
*/

char BCMuartReadByte_NI() {
    return *AUX_UART_IO;
}

char BCMuartReadByte() {
    while ((*AUX_UART_LINE_STATUS & RX_FIFO_EMPTY) == 0)  { /* waits */ }
    return *AUX_UART_IO;
}

// ----------------------- //

#endif
