#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// test013: double free.

int main() {
    void *ptr = malloc(2001);
    free(ptr);
    free(ptr);
    m61_printstatistics();
}

//! MEMORY BUG???: double free of pointer ???
//! ???
