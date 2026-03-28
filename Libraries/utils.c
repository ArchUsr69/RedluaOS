#include <stdint.h>
#include <stddef.h>
// --------------- //
#include <utils.h>

inline void memorySet(REGISTER_32 *target, uint32_t value, size_t length) {
    for (uint32_t offset; offset < (length / 4); offset++) {
        target[offset] = value;
    }
}

inline void memoryCopy(REGISTER_32 *target, REGISTER_32 *origin, size_t length) {
    for (uint32_t offset; offset < (length / 4); offset++) {
        target[offset] = origin[offset];
    }
}

