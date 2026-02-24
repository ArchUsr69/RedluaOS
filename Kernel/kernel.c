#include <stdint.h>
#include <stdbool.h>
#include "../Libraries/lowLevel.h"
#include "../Hardware/bcm2835/bcm2835.h"

// Test Code, does nothing meaningfull yet
void kernel_main() {
    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_pinWrite(ACT_LED, HIGH);
}
