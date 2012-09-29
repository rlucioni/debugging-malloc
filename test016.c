#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// test016: file name and line number of double free.

int main() {
    void *ptr = malloc(2001);
    free(ptr);
    for (int i = 0; i < 10; ++i)
	/* do nothing */;
    free(ptr);
    m61_printstatistics();
}

//! MEMORY BUG: test016.c:12: double free of pointer ???
//!   test016.c:9: pointer ??? previously freed here
//! ???
