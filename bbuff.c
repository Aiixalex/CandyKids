#include "bbuff.h"
#include <stdio.h>
#include <stdbool.h>

void* buffer_data[BUFFER_SIZE];
int in, out, count = 0;

void bbuff_blocking_insert(void* item) {
    if (count == BUFFER_SIZE) {
        return;
    }
    buffer_data[in] = item;
    
    in = (in + 1) % BUFFER_SIZE;
    count++;
}

void* bbuff_blocking_extract(void) {
    if (count == 0) {
        return NULL;
    }
    void* extracted = buffer_data[out];
    
    out = (out + 1) % BUFFER_SIZE;
    count--;

    return extracted;
}

_Bool bbuff_is_empty(void) {
    if (count == 0) {
        return true;
    } else {
        return false;
    }
}