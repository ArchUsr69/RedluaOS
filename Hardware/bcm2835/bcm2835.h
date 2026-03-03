#include <stdint.h>
#include <stdbool.h>
#include <framebuffer.h>
#include <lowLevel.h>
#include <gpio.h>

#ifndef BCM2835_H
#define BCM2835_H

//==========GPIO==========//

#define GPIO_BASE 0x20200000

// GPIO Pin Function selector register base;
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

// Misc
#define TOTAL_PINS 53
#define REGISTER_SIZE 32

/*
-> maps the standard gpio function numbers to the bcm2835 gpio function values
*/

static const uint8_t bcm2835gpio_functions[8] = {
    0b000, // Input
    0b001, // Output
    0b100, // Alternative 0
    0b101, // Alternative 1
    0b110, // Alternative 2
    0b111, // Alternative 3
    0b011, // Alternative 4
    0b010, // Alternative 5
};

// ------------------------- //

// Function prototypes
void bcm2835gpio_setFunction(uint8_t pin, enum gpio_functions function);
void bcm2835gpio_pinWrite(uint8_t pin, bool level);

//==========Mailbox==========//

#define MAILBOX_BASE 0x2000B880
#define MAILBOX_CHANNEL 8

// Mailbox 0 Registers (*NEVER WRITE TO THESE REGISTERS*)
#define MAILBOX_READ ((volatile uint32_t *)(MAILBOX_BASE + 0x00))
#define MAILBOX_READ_PEEK ((volatile uint32_t *)(MAILBOX_BASE + 0x10))
#define MAILBOX_READ_SENDER ((volatile uint32_t *)(MAILBOX_BASE + 0x14))
#define MAILBOX_READ_STATUS ((volatile uint32_t *)(MAILBOX_BASE + 0x18))
#define MAILBOX_READ_CONFIG ((volatile uint32_t *)(MAILBOX_BASE + 0x1C))

// Mailbox 1 Registers (You shouldn't read these registers; not that you have to)
#define MAILBOX_WRITE ((volatile uint32_t *)(MAILBOX_BASE + 0x20))
#define MAILBOX_WRITE_PEEK ((volatile uint32_t *)(MAILBOX_BASE + 0x30))
#define MAILBOX_WRITE_SENDER ((volatile uint32_t *)(MAILBOX_BASE + 0x34))
#define MAILBOX_WRITE_STATUS ((volatile uint32_t *)(MAILBOX_BASE + 0x38))
#define MAILBOX_WRITE_CONFIG ((volatile uint32_t *)(MAILBOX_BASE + 0x3C))

// Mailbox Status Flags
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

// Buffer status Flags
#define BUFFER_PARSE_SUCCESS 0x80000000
#define BUFFER_PARSE_FAILURE 0x80000001

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

/*
-> standard mailbox buffer structure; 
-> size must be calculated;
-> request_response must always be 0 initialized; VC will overwrite with 0x80000000 for success, 0x80000001 for failure;
*/

static struct mailboxBuffer {
    uint32_t size;
    uint32_t request_response;
    uint32_t tags[64];
};

// ---------------------------- //

// Function prototypes
static void mailboxWrite(struct mailboxBuffer *pointer);
static uint32_t mailboxRead();
void mailboxFramebufferInit(struct framebuffer_metadata *pointer);

#endif
