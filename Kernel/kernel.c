#include <stdint.h>
#include <stddef.h>
#include "../Libraries/lowLevel.h"
#include "../Hardware/bcm2835/bcm2835.h"

// This is just a test Program which ligths an LED open on Pin 11. It just works

int kernel_main(void) {
    gpio_setFunction(17, OUTPUT);
    gpio_pinOutput(17);
}
