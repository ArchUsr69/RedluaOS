#include "BCM2835.h"

// as this is just the first Test Program, i'm too lazy to
// Explain how and why (it's just a Test Code anyway) this Code is like this

void gpio_init() {
  unsigned int tempVar;

  tempVar = *GPFSEL1;
  tempVar &= ~(7 <<  21);
  tempVar |= (1 << 21);
  *GPFSEL1 = tempVar;
}

void led_on() {
  *GPSET0 = (1 << 17);
}
