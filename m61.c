#define M61_DISABLE 1
#include "m61.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#define BOUNDARY_WRITE_BYTES 4
#define SIZE_BUCKETS 32
#define TERMINAL_WIDTH 80
#define HH_THRESHOLD 0.25


static struct m61_statistics cs61_stats;
static void *latest_ptr = NULL;
static void *heap_min   = NULL;
static void *heap_max   = NULL;

void update_bounds(void *ptr, size_t sz) {
    if (ptr < heap_min || heap_min == NULL)
        heap_min = ptr;
    if (ptr + sz > heap_max)
        heap_max = ptr + sz;
}

int out_of_bounds(void *ptr) {
    return !heap_min || ptr < heap_min || ptr > heap_max;
}

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
