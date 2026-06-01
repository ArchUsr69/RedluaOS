#include <redlib.h>

#include <Kernel/uart.h>

#ifdef BCM2712

#include <Drivers/broadcom.h>

struct uart Uart = {
    .init = BCMuartInit,
    .readByte = BCMuartReadByte_NI,
    .baudrate = 115200
};

#endif
