#include <types.h>

#ifndef UTILS_H
#define UTILS_H

typedef volatile uint8 *MMIO_8;
typedef volatile uint16 *MMIO_16;
typedef volatile uint32 *MMIO_32;

typedef const volatile uint8 *RO_MMIO_8;
typedef const volatile uint16 *RO_MMIO_16;
typedef const volatile uint32 *RO_MMIO_32;

#define alignas _Alignas
#define alignof _Alignof

#define ALIGNED(x) __attribute__((aligned(x)))
#define PACKED __attribute__((packed))
#define OPTIMIZE(x) __attribute__((optimize(x)))

#define HIGH true
#define LOW false

#define NULL ((void *)0)

void memorySet(void *target, uint8 value, size_t length);
void memoryCopy(void *target, void *source, size_t length);
int memoryCompare(void *source1, void *source2, size_t length);
void *memorySearch(void *source, uint8 value, size_t length);

#endif
