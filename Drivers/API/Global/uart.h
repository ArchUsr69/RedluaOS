#include <types.h>
#include <string.h>

#ifndef UART_H
#define UART_H

// Driver Table;
struct uart {
    void (*init)();
    void (*writeByte)(char byte);
    char (*readByte)();
    void (*writeText)(string string);
};

extern struct uart Uart;

#endif
