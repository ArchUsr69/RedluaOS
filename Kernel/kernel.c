#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <framebuffer.h>

struct framebuffer_table mailbox_framebuffer = {
    .framebufferInit = mailboxFramebufferInit
};

struct gpio_table bcm_gpio_table = {
    .setFunction = bcm2835gpio_setFunction,
    .pinWrite = bcm2835gpio_pinWrite
};

struct gpio_table *gpio = &bcm_gpio_table;
struct framebuffer_table *framebuffer = &mailbox_framebuffer;
struct framebuffer_metadata framebuffer_metadata;

// Still Test code;
void kernel_main() {
    bool status = test();
    if (status == 1) {
        gpio_setFunction(PWR_LED, OUTPUT);
        gpio_pinWrite(PWR_LED, LOW);
    } else if (status == 2) {
        gpio_setFunction(ACT_LED, OUTPUT);
        gpio_pinWrite(ACT_LED, HIGH);
    }
}
