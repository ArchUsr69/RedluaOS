#include <types.h>
#include <string.h>

#ifndef UART_H
#define UART_H

// Driver Table;
struct uartTable {
    void (*uartInit)();
    void (*uartWriteByte)(char byte);
    char (*uartReadByte)();
    void (*uartWriteText)(string string);
};

extern struct uartTable UartTable;

// Wrapper functions;
static inline void uartInit() {
    UartTable.uartInit();
}

static inline void uartWriteByte(char byte) {
    UartTable.uartWriteByte(byte);
}

static inline char uartReadByte() {
    return UartTable.uartReadByte();
}

static inline void uartWriteText(string string) {
    UartTable.uartWriteText(string);
}

#endif
