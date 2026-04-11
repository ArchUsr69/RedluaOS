#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <broadcom.h>

#define AUX_BASE (PERIPHERAL_BASE + 0x215000)
#define AUX_ENABLE ((MMIO_8)(AUX_BASE + 0x04))

#define AUX_UART_IO ((MMIO_32)(AUX_BASE + 0x40))
#define AUX_UART_INTERUPT ((MMIO_32)(AUX_BASE + 0x44))
#define AUX_UART_INTERUPT_ID ((MMIO_32)(AUX_BASE + 0x48))
#define AUX_UART_LINE_CONTROL ((MMIO_32)(AUX_BASE + 0x4C))
#define AUX_UART_MODEM_CONTROL ((MMIO_32)(AUX_BASE + 0x50))
#define AUX_UART_LINE_STATUS ((MMIO_32)(AUX_BASE + 0x54))
#define AUX_UART_EXTRA_CONTROL ((MMIO_32)(AUX_BASE + 0x60))
#define AUX_UART_EXTRA_STATUS ((MMIO_32)(AUX_BASE + 0x64))
#define AUX_UART_BAUD ((MMIO_32)(AUX_BASE + 0x68))


#define GPIO_BASE (PERIPHERAL_BASE + 0x200000)
#define GPIO_PULL_CONTROL ((MMIO_32)(GPIO_BASE + 0x94))
#define GPIO_PULL_CLOCK_0 ((MMIO_32)(GPIO_BASE + 0x98))

static inline void delayCycles(uint32 cycles) {
    while (cycles--) {
        asm volatile("nop");
    }
}

void BCMuartInit() {
    *AUX_ENABLE = 1U;
    *AUX_UART_LINE_CONTROL = 0x81;
    *AUX_UART_BAUD = 3250;
    *AUX_UART_LINE_CONTROL = 0x01;
    
    gpioSetFunction(14, ALTERNATIVE5);
    gpioSetFunction(15, ALTERNATIVE5);

    *GPIO_PULL_CONTROL = 0;
    delayCycles(150);

    *GPIO_PULL_CLOCK_0 = (1U << 14) | (1U << 15);
    delayCycles(150);

    *GPIO_PULL_CLOCK_0 = 0;
}

void BCMuartWriteByte(char byte) {
    while((*AUX_UART_LINE_STATUS & 0x20) == 0) {}
    *AUX_UART_IO = byte;
}

char BCMuartReadByte() {
    while ((*AUX_UART_LINE_STATUS & 1U) == 0)  { /* waits */ }
    return *AUX_UART_IO;
}

void BCMuartWriteText(string string) {
    size_t index = 0;

    while (string.text[index] != '\0') {
        if (string.text[index] == '\n') {
            BCMuartWriteByte('\r');
        }

        BCMuartWriteByte(string.text[index++]);
    }
}
