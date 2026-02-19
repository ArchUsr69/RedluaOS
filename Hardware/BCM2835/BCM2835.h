#ifndef BCM2835_H
#define BCM2835_H

// I decided, i should keep everything Hardware specific to be separated
// from the rest of the Kernel logic (which resides in the Kernel Subsystem).
// it's the central Design Choice of this OS, as it will spare me headaches with 
// porting it to other Devices later 

//-------------GPIO-------------//

#define GPIO_BASE 0x20200000

// GPIO Pin Function selector
#define GPFSEL0 ((volatile unsigned int *)(GPIO_BASE + 0x00))
#define GPFSEL1 ((volatile unsigned int *)(GPIO_BASE + 0x04))
#define GPFSEL2 ((volatile unsigned int *)(GPIO_BASE + 0x08))
#define GPFSEL3 ((volatile unsigned int *)(GPIO_BASE + 0x0C))
#define GPFSEL4 ((volatile unsigned int *)(GPIO_BASE + 0x10))
#define GPFSEL5 ((volatile unsigned int *)(GPIO_BASE + 0x14))

// Output set Registers
#define GPSET0 ((volatile unsigned int *)(GPIO_BASE + 0x1C))
#define GPSET1 ((volatile unsigned int *)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPCLR0 ((volatile unsigned int *)(GPIO_BASE + 0x28))
#define GPCLR1 ((volatile unsigned int *)(GPIO_BASE + 0x2C))

void gpio_init();
void led_on();

#endif
