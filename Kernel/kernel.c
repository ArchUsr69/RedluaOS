#include <stdint.h>
#include <stdbool.h>
#include "../Libraries/lowLevel.h"
#include "../Hardware/bcm2835/bcm2835.h"

void kernel_main() {
    gpio_setFunction(47, OUTPUT);
    gpio_pinOutput(47);
}
