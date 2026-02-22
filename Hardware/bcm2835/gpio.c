#include <stdint.h>
#include <stdbool.h>
#include "bcm2835.h"
#include "../../Libraries/lowLevel.h"

// basically just like the Arduino version of pinMode()
void gpio_setFunction(uint8_t pin, uint8_t function) {
  uint8_t start = (pin % 10) * 3;
  volatile uint32_t *target = GPIOFSELECT_BASE + (pin / 10);
  bit32_setSection(target, function, start, 3);
}

// and this one just the Arduino version of digitalWrite(pin, OUTPUT)
void gpio_pinOutput(uint8_t pin) {
  if (pin < 32) {
    bit32_setOn(GPIOSET0, pin);
  }
  else {
    bit32_setOn(GPIOSET1, pin - 32);
  }
}
