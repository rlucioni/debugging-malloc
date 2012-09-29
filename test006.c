#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
// test006: failed allocation.

int main() {
    void *ptrs[10];
    for (int i = 0; i < 10; ++i)
	ptrs[i] = malloc(i + 1);
    for (int i = 0; i < 5; ++i)
	free(ptrs[i]);
    size_t very_large_size = (size_t) -1 - 150;
    void *garbage = malloc(very_large_size);
    assert(!garbage);
    m61_printstatistics();
}

// The text within ??{...}?? pairs is a regular expression.
// We're looking for a `fail_size` of either 2^32 - 151 or 2^64 - 151.
// (Two rather ridiculous sites about regular expressions:
//  http://www.regular-expressions.info/quickstart.html
//  http://xkcd.com/208/ )

//! malloc count: active          5   total         10   fail          1
//! malloc size:  active         40   total         55   fail ??{4294967145|18446744073709551465}??
