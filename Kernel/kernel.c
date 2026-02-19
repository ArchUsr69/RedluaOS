#include "../Hardware/BCM2835/BCM2835.h"

// This is just a test Program which ligths an LED open on Pin 11. It just works

int kernel_main(void) {
    gpio_init();
    led_on();
}
