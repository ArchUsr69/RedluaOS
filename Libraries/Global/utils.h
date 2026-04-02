#include <types.h>

#ifndef UTILS_H
#define UTILS_H

typedef volatile uint8 *REGISTER_8;
typedef volatile uint16 *REGISTER_16;
typedef volatile uint32 *REGISTER_32;

typedef const volatile uint8 *R_REGISTER_8;
typedef const volatile uint16 *R_REGISTER_16;
typedef const volatile uint32 *R_REGISTER_32;

#define alignas _Alignas
#define alignof _Alignof

#define ALIGNED(x) __attribute__((aligned(x)))
#define PACKED __attribute__((packed))
#define OPTIMIZE(x) __attribute__((optimize(x)))

#define HIGH true
#define LOW false

#define NULL ((void *)0)

#endif
