#include <stdint.h>
#include "bcm2835.h"
#include "../../Libraries/lowLevel.h"

// This file includes the functionality of the GPIO Driver for the BCM2835

void gpio_setFunction(uint8_t pin, uint8_t function) {
  uint8_t start = (pin % 10) * 3;
  volatile uint32_t *target = GPIOFSELECT_BASE + (pin / 10);
  bit32_setSection(target, function, start, 3);
}

void gpio_pinOutput(uint8_t pin) {
  if (pin < 32) {
    bit32_setOn(GPIOSET0, pin);
  }
  else {
    bit32_setOn(GPIOSET1, pin - 32);
  }
}
