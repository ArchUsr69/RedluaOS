#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>

struct framebuffer_table mailbox_framebuffer = {
    .framebuffInit = mailboxFramebufferInit
};

struct gpio_table bcm_gpio_table = {
    .setFunction = bcm2835gpio_setFunction,
    .pinWrite = bcm2835gpio_pinWrite
};

struct gpio_table *gpio = &bcm_gpio_table;
struct framebuffer_table *framebuffer = &mailbox_framebuffer;

// Still Test code;
void kernel_main() {
    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_pinWrite(ACT_LED, HIGH);
}
