#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// test014: wild free inside heap-allocated data.

int main() {
    void *ptr = malloc(2001);
    free((char *) ptr + 100);
    m61_printstatistics();
}

//! MEMORY BUG???: invalid free of pointer ???, not allocated
//! ???
