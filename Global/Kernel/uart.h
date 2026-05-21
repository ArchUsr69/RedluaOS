#include <redlib.h>

#ifndef UART_H
#define UART_H

// Driver Table;
struct uart {
    void (*init)();
    void (*writeByte)(char byte);
    char (*readByte)();
    void (*writeText)(string string);
    uint32 baudrate;
};

extern struct uart Uart;

#endif
