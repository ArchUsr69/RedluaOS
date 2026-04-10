#include <types.h>
#include <string.h>

#ifndef UART_H
#define UART_H

struct uartTable {
    void (*uartInit)();
    void (*uartWriteByte)(char byte);
    char (*uartReadByte)();
    void (*uartWriteText)(string string);
};

extern struct uartTable GlobalUartTable;

static inline void uartInit() {
    GlobalUartTable.uartInit();
}

static inline void uartWriteByte(char byte) {
    GlobalUartTable.uartWriteByte(byte);
}

static inline char uartReadByte() {
    return GlobalUartTable.uartReadByte();
}

static inline void uartWriteText(string string) {
    GlobalUartTable.uartWriteText(string);
}

#endif
