#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <broadcom.h>

#define UART_BASE (PERIPHERAL_BASE + 0x215000)

#define UART_ENABLE ((MMIO_8)(PERIPHERAL_BASE + 0x04))
#define UART_DATA ((MMIO_8)(PERIPHERAL_BASE + 0x40))
#define UART_INTERUPT ((MMIO_8)(PERIPHERAL_BASE + 0x44))
#define UART_INTERUPT_ID ((MMIO_8)(PERIPHERAL_BASE + 0x48))
#define UART_MODEM_CONTROL ((MMIO_32)(PERIPHERAL_BASE + 0x50))
#define UART_LINE_STATUS ((MMIO_8)(PERIPHERAL_BASE + 0x54))
#define UART_LINE_CONTROL ((MMIO_8)(PERIPHERAL_BASE + 0x4C))
#define UART_BAUD ((MMIO_16)(PERIPHERAL_BASE + 0x68))
#define UART_EXTRA_STATUS ((MMIO_32)(PERIPHERAL_BASE + 0x64))
#define UART_EXTRA_CONTROL ((MMIO_8)(PERIPHERAL_BASE + 0x60))

#define GPIO_BASE (PERIPHERAL_BASE + 0x200000)
#define GPIO_PULL_CONTROL ((MMIO_32)(GPIO_BASE + 0x94))
#define GPIO_PULL_CLOCK_0 ((MMIO_32)(GPIO_BASE + 0x98))

static inline void delayCycles(uint32 cycles) {
    while (cycles--) {
        asm volatile("nop");
    }
}

void BCMuartInit() {
    *UART_ENABLE |= 1;
    *UART_EXTRA_CONTROL = 0;
    *UART_LINE_CONTROL = 3;
    *UART_MODEM_CONTROL = 0;
    *UART_INTERUPT = 0;
    *UART_INTERUPT_ID = 0xC6;
    *UART_BAUD = 9113;
    
    gpioSetFunction(14, ALTERNATIVE0);
    gpioSetFunction(15, ALTERNATIVE0);

    *GPIO_PULL_CONTROL = 0;
    delayCycles(150);

    *GPIO_PULL_CLOCK_0 = (1U << 14) | (1U << 15);
    delayCycles(150);

    *GPIO_PULL_CLOCK_0 = 0;
}

void BCMuartWriteByte(char byte) {
    while ((*UART_LINE_STATUS & 0x20) == 0) { /* waits */ }
    *UART_DATA = byte;
}

char BCMuartReadByte() {
    while ((*UART_LINE_STATUS & 0x01) == 0)  { /* waits */ }
    return *UART_DATA;
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
