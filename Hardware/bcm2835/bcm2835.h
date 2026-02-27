#include <stdint.h>
#include <stdbool.h>
#include "../../Libraries/lowLevel.h"

#ifndef BCM2835_H
#define BCM2835_H

// A Library which stores every specific aspect of the BCM2835 Hardware in one place

//==========GPIO==========//

#define GPIO_BASE 0x20200000

// GPIO Pin Function selector
#define GPIOFSELECT_BASE ((volatile uint32_t *)(GPIO_BASE))
#define GPIOFSELECT_OFFSET 0x04

// GPIO Functions
#define INPUT 0b000
#define OUTPUT 0b001
#define FUNCTION_0 0b100
#define FUNCTION_1 0b101
#define FUNCTION_2 0b110
#define FUNCTION_3 0b111
#define FUNCTION_4 0b011
#define FUNCTION_5 0b010

// Output set Registers
#define GPIOSET0 ((volatile uint32_t *)(GPIO_BASE + 0x1C))
#define GPIOSET1 ((volatile uint32_t *)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPIOCLEAR0 ((volatile uint32_t *)(GPIO_BASE + 0x28))
#define GPIOCLEAR1 ((volatile uint32_t *)(GPIO_BASE + 0x2C))

// ACT_LED gpio pin number
#define ACT_LED 47

// Functions
void gpio_setFunction(uint8_t pin, uint8_t function);
void gpio_pinWrite(uint8_t pin, bool level);

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

// Mailbox tags
#define FRAMEBUFFER_ALLOCATE 0x00040001
#define FRAMEBUFFER_RELEASE 0x00048001
#define SCREEN_BLANK 0x00040002
#define FRAMEBUFFER_GET_PHYSICAL_SIZE 0x00040003
#define FRAMEBUFFER_TEST_PHYSICAL_SIZE 0x00044003
#define FRAMEBUFFER_SET_PHYSICAL_SIZE 0x00048003
#define FRAMEBUFFER_GET_VIRTUAL_SIZE 0x00040004
#define FRAMEBUFFER_TEST_VIRTUAL_SIZE 0x00044004
#define FRAMEBUFFER_SET_VIRTUAL_SIZE 0x00048004
#define GET_DEPTH 0x00040005
#define TEST_DEPTH 0x00044005
#define SET_DEPTH 0x00048005
#define GET_PITCH 0x00040008
#define GET_VIRTUAL_OFFSET 0x00040009
#define TEST_VIRTUAL_OFFSET 0x00044009
#define SET_VIRTUAL_OFFSET 0x00048009
#define GET_PIXELORDER 0x00040006
#define TEST_PIXELORDER 0x00044006
#define SET_PIXELORDER 0x00048006

struct mailboxTag {
    uint32_t tag_id;
    uint32_t size;
    uint32_t requested_length;
    uint32_t data[];
}

struct mailboxBuffer {
    uint32_t size = 0;
    uint32_t request = 0;
    uint32_t *tags[];
}

uint32_t physicalToBus(uint32_t *address, bool l2Cache_enabled);
uint32_t busToPhysical(uint32_t *address, bool l2Cache_enabled);
void mailboxWrite(uint8_t channel, uint32_t data);
uint32_t mailboxRead(uint8_t channel);

#endif
