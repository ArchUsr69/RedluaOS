#include <types.h>
#include <utils.h>

void memorySet(uint32 *target, uint32 value, uint32 length) {
    for (uint32 offset; offset < (length / 4); offset++) {
        target[offset] = value;
    }
}

void memoryCopy(uint32 *target, uint32 *origin, uint32 length) {
    uintptr a = (uintptr)target;
    uintptr b = (uintptr)origin;
    uintptr difference = (a > b) ? (a - b) : (b - a);
    if (difference > length) return;
    for (uint32 offset; offset < (length / 4); offset++) {
        target[offset] = origin[offset];
    }
}
