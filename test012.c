#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// test012: free of invalid pointer.

int main() {
    free((void *) 1);
    m61_printstatistics();
}

//! MEMORY BUG???: invalid free of pointer ???, not in heap
//! ???
