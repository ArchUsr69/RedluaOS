#include <stdint.h>
#include <stdbool.h>
#include "lowLevel.h"
#include "gpio.h"

#ifndef BCM2835_H
#define BCM2835_H

// All the BCM2835 Driver details in one header file;

//==========GPIO==========//

#define GPIO_BASE 0x20200000

//GPIO Pin Function selector register base;
#define GPIOFSELECT_BASE ((volatile uint32_t *)(GPIO_BASE))

// Output set Registers
#define GPIOSET0 ((volatile uint32_t *)(GPIO_BASE + 0x1C))
#define GPIOSET1 ((volatile uint32_t *)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPIOCLEAR0 ((volatile uint32_t *)(GPIO_BASE + 0x28))
#define GPIOCLEAR1 ((volatile uint32_t *)(GPIO_BASE + 0x2C))

// ACT_LED and PWR_LED gpio pin number
#define ACT_LED 47
#define PWR_LED 35

// Functions
void bcm2835gpio_setFunction(uint8_t pin, enum gpio_functions);
void bcm2835gpio_pinWrite(uint8_t pin, bool level);

//==========Mailbox==========//

#define MAILBOX_BASE 0x2000B880

// Mailbox Registers
#define MAILBOX_READ ((volatile uint32_t *)(MAILBOX_BASE + 0x00))
#define MAILBOX_PEEK ((volatile uint32_t *)(MAILBOX_BASE + 0x04))
#define MAILBOX_SENDER ((volatile uint32_t *)(MAILBOX_BASE + 0x08))
#define MAILBOX_STATUS ((volatile uint32_t *)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE ((volatile uint32_t *)(MAILBOX_BASE + 0x20))
#define MAILBOX_CONFIG ((volatile uint32_t *)(MAILBOX_BASE + 0x1C))

// Mailbox Status
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

// Mailbox Tags
enum mailboxTags {
    FRAMEBUFFER_ALLOCATE = 0x00040001,
    FRAMEBUFFER_RELEASE = 0x00048001,
    SCREEN_BLANK = 0x00040002,
    GET_PHYSICAL_SIZE = 0x00040003,
    TEST_PHYSICAL_SIZE = 0x00044003,
    SET_PHYSICAL_SIZE = 0x00048003,
    GET_VIRTUAL_SIZE = 0x00040004,
    TEST_VIRTUAL_SIZE = 0x00044004,
    SET_VIRTUAL_SIZE = 0x00048004,
    GET_DEPTH = 0x00040005,
    TEST_DEPTH = 0x00044005,
    SET_DEPTH = 0x00048005,
    GET_PITCH = 0x00040008,
    GET_VIRTUAL_OFFSET = 0x00040009,
    TEST_VIRTUAL_OFFSET = 0x00044009,
    SET_VIRTUAL_OFFSET = 0x00048009,
    GET_PIXELORDER = 0x00040006,
    TEST_PIXELORDER = 0x00044006,
    SET_PIXELORDER = 0x00048006
};

static void mailboxWrite(struct mailboxBuffer *pointer);
static uint32_t mailboxRead();
void mailboxFramebufferInit(struct framebuffer_metadata *pointer);

#endif
