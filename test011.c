#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// test011: wild free.

int main() {
    int x;
    free(&x);
    m61_printstatistics();
}

//! MEMORY BUG???: invalid free of pointer ???, not in heap
//! ???
