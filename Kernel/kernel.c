#include <stdint.h>
#include <stdbool.h>
#include "../Libraries/lowLevel.h"
#include "../Hardware/bcm2835/bcm2835.h"

// Test Code, does nothing meaningfull yet
void kernel_main() {
    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_setFunction(PWR_LED, OUTPUT);
    uint32_t status = mailboxFramebufferInit();
    if (status == 0x80000000) {
        gpio_pinWrite(ACT_LED, HIGH);
    } else if (status == 0x80000001) {
        gpio_pinWrite(PWR_LED, LOW);
    }
}

