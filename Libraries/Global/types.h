#ifndef TYPES_H
#define TYPES_H

// signed integer types and constants;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

#define INT8_MIN (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483648)

#define INT8_MAX (127)
#define INT16_MAX (32767)
#define INT32_MAX (2147483647)

#define INT8_BYTES (1)
#define INT16_BYTES (2)
#define INT32_BYTES (4)

#define INT8_WIDTH (8)
#define INT16_WIDTH (16)
#define INT32_WIDTH (32)

// Unsigned integer types and constants;
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

#define UINT8_MAX (255)
#define UINT16_MAX (65535)
#define UINT32_MAX (4294967295)

// integer type for pointer values and sizes;
typedef uint32 uintptr;
typedef uint32 size_t;

// boolean types
#define true ((bool)(1U))
#define false ((bool)(0))

#endif
