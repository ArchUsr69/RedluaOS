#include <types.h>
#include <utils.h>

void memorySet(void *target, uint8 value, size_t length) {
    uint8 *pointer = target;
    for (size_t offset = 0; offset < length; offset++) {
        pointer[offset] = value;
    }
}

/*
-> IMPORTANT: it checks if both sources overlap;
*/

void memoryCopy(void *target, void *source, size_t length) {
    uintptr a = (uintptr)target;
    uintptr b = (uintptr)source;

    uintptr difference = (a > b) ? (a - b) : (b - a);
    if (difference > length) return;

    uint8 *pointerTarget = target;
    uint8 *pointerOrigin = source;

    for (size_t offset = 0; offset < length; offset++) {
        pointerTarget[offset] = pointerOrigin[offset];
    }
}

// ---------------------------------- //

/*
-> 1 : source1 is bigger than source2;
-> 0 : both are equal;
-> -1 : source1 is smaller than source2;
*/

int memoryCompare(void *source1, void *source2, size_t length) {
    uint8 *pointerSource1 = source1;
    uint8 *pointerSource2 = source2;
    
    for (size_t offset = 0; offset < length; offset++) {
        if (pointerSource1[offset] != pointerSource2[offset]) {
            if (pointerSource1[offset] > pointerSource2[offset]) return 1;
            else return -1;
        }
    }

    return 0;
}

// ------------------------------- //

void *memorySearch(void *source, uint8 value, size_t length) {
    uint8 *pointer = source;

    for (size_t offset = 0; offset < length; offset++) {
        if (pointer[offset] == value) return (pointer + offset);
    }

    return NULL;
}
