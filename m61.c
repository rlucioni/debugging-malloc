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

typedef struct {
    void *prev;
    void *next;
    size_t size;
    char *filename;
    int line;
    void *cur;
    char allocated;
    int frequency;
    // for the size linked list
    void *hh_size_next;
    // for the num allocations frequency linked list
    void *hh_freq_next;
    //char padding[7];
} alloc_info;

// setters and getter for the struct
void *get_prev(void *ptr) {
    return (((alloc_info*)ptr) - 1)->prev;
}

void *get_next(void *ptr) {
    return (((alloc_info*)ptr) - 1)->next;
}

size_t get_size(void *ptr) {
    return (((alloc_info)ptr) - 1)->size;
}

// const char *get_filename(void *ptr) {
char *get_filename(void *ptr) {
    return (((alloc_info*)ptr) - 1)-> filename;
}

int get_line(void *ptr) {
    return (((alloc_info*)ptr) - 1)->line;
}

void *get_cur(void *ptr) {
    return (((alloc_info*)ptr) - 1)->cur;
}

int is_allocated(void* ptr) {
    return (((alloc_info*)ptr) - 1)->allocated && get_cur(ptr) == ptr;
}

int get_frequency(void *ptr) {
    return (((alloc_info*)ptr) - 1)->frequency;
}

void *get_hh_size_next(void *ptr) {
    return (((alloc_info*)ptr) - 1)->hh_size_next;
}

void set_prev(void *ptr, void *prev) {
    (((alloc_info*)ptr) - 1)->prev = prev;
}

void set_next(void *ptr, void *next) {
    (((alloc_info*)ptr) - 1)->next = next;
}

void set_size(void *ptr, size_t size) {
    (((alloc_info)ptr) - 1)->size = size;
}

// void set_filename (void *ptr, const char *filename) ->> requires cast char*
void set_filename(void *ptr, char *filename) {
    (((alloc_info*)ptr) - 1)->filename = filename;
}

void set_line(void *ptr, int line) {
    (((alloc_info*)ptr) - 1)->line = line;
}

void set_cur(void *ptr, void *cur) {
    (((alloc_info*)ptr) - 1)->cur = cur;
}

void set_allocated(void *ptr, int alloc_status) {
    (((alloc_info*)ptr) - 1)->allocated = alloc_status;
}

void set_frequency(void *ptr, int frequency) {
    (((alloc_info*)ptr) - 1)->frequency = frequency;
}

void set_hh_size_next(void *ptr, void *hh_size_next) {
    (((alloc_info*)ptr) - 1)->hh_size_next = hh_size_next;
}


// prototype for a heavy hitter function
void insert_node(void *ptr);

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
