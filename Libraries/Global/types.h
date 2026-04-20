#ifndef TYPES_H
#define TYPES_H

// signed integer types and constants;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

#define INT8_MIN (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483648)
#define INT64_MIN (-9223372036854775808)

#define INT8_MAX (127)
#define INT16_MAX (32767)
#define INT32_MAX (2147483647)
#define INT64_MAX (9223372036854775807)

#define INT8_BYTES (1)
#define INT16_BYTES (2)
#define INT32_BYTES (4)
#define INT64_BYTES (8)

#define INT8_WIDTH (8)
#define INT16_WIDTH (16)
#define INT32_WIDTH (32)
#define INT64_WIDTH (64)

// Unsigned integer types and constants;
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#define UINT8_MAX (255)
#define UINT16_MAX (65535)
#define UINT32_MAX (4294967295)
#define UINT64_MAX (18446744073709551615)

// integer type for pointer values and sizes;

#ifdef __arch64__
    typedef uint64 uintptr;
    typedef uint64 size_t;
#else 
    typedef uint32 uintptr;
    typedef uint32 size_t;
#endif

// boolean types
#define true ((bool)(1))
#define false ((bool)(0))

#endif
