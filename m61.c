#define M61_DISABLE 1
#include "m61.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

void *m61_malloc(size_t sz, const char *file, int line) {
    (void) file, (void) line;	// avoid uninitialized variable warnings
    // Your code here.
    return malloc(sz);
}

void m61_free(void *ptr, const char *file, int line) {
    (void) file, (void) line;	// avoid uninitialized variable warnings
    // Your code here.
    free(ptr);
}

void *m61_realloc(void *ptr, size_t sz, const char *file, int line) {
    (void) file, (void) line;	// avoid uninitialized variable warnings
    // Your code here.
    return realloc(ptr, sz);
}

void *m61_calloc(size_t nmemb, size_t sz, const char *file, int line) {
    (void) file, (void) line;	// avoid uninitialized variable warnings
    // Your code here.
    return calloc(nmemb, sz);
}

void m61_getstatistics(struct m61_statistics *stats) {
    // Stub: set all statistics to 0
    memset(stats, 0, sizeof(struct m61_statistics));
    // Your code here.
}

void m61_printstatistics(void) {
    struct m61_statistics stats;
    m61_getstatistics(&stats);

    printf("malloc count: active %10llu   total %10llu   fail %10llu\n\
malloc size:  active %10llu   total %10llu   fail %10llu\n",
	   stats.active_count, stats.total_count, stats.fail_count,
	   stats.active_size, stats.total_size, stats.fail_size);
}

void m61_printleakreport(void) {
    // Your code here.
}
