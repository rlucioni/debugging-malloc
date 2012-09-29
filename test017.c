#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// test017: check different file name support.
extern void my_free(void *);

int main() {
    void *ptr = malloc(2001);
    my_free(ptr);
    for (int i = 0; i < 10; ++i)
	/* do nothing */;
    free(ptr);
    m61_printstatistics();
}

//! MEMORY BUG: test017.c:13: double free of pointer ???
//!   test017-help.c:4: pointer ??? previously freed here
//! ???
