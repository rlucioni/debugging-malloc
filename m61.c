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
    return (((alloc_info*)ptr) - 1)->size;
}

const char *get_filename(void *ptr) {
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
    (((alloc_info*)ptr) - 1)->size = size;
}

void set_filename(void *ptr, const char *filename) {
    (((alloc_info*)ptr) - 1)->filename = (char*) filename;
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
//void insert_node(void *ptr);

void *m61_malloc(size_t sz, const char *file, int line) {
    // avoid uninitialized variable warnings
    (void) file, (void) line;
    
    size_t total_alloc_sz = sz + sizeof(alloc_info)
                               + BOUNDARY_WRITE_BYTES
                               + sizeof(void*);

    if ((int)sz < 0 || total_alloc_sz < sz) {
        cs61_stats.fail_count++;
        cs61_stats.fail_size += sz;
        return NULL;
    }

    void *ptr = malloc(total_alloc_sz);

    if (ptr == NULL) {
        cs61_stats.fail_count++;
        cs61_stats.fail_size += sz;
        return ptr;
    }

    else {
        cs61_stats.active_count++;
        cs61_stats.active_size += sz;
        cs61_stats.total_count++;
        cs61_stats.total_size += sz;

        memset((ptr + sizeof(alloc_info) + sz), ~0, BOUNDARY_WRITE_BYTES);

        set_size(((alloc_info*)ptr) + 1, sz);
        set_prev(((alloc_info*)ptr) + 1, latest_ptr);
        set_next(((alloc_info*)ptr) + 1, NULL);

        if (latest_ptr != NULL)
            set_next(latest_ptr, ((alloc_info*)ptr) + 1);

        latest_ptr = ((alloc_info*)ptr) + 1;

        update_bounds(latest_ptr, sz);
        set_filename(latest_ptr, file);
        set_line(latest_ptr, line);
        set_cur(latest_ptr, latest_ptr);
        set_allocated(latest_ptr, 1);

        // for heavy hitters
        //insert_node(latest_ptr);

        return (latest_ptr);
    }
}

void boundary_write_detect(void *ptr, const char *file, int line) {
    int payload_bytes = get_size(ptr);
    void *boundary = (char*)ptr + payload_bytes;

    for (int i = 0; i < BOUNDARY_WRITE_BYTES; i++) {
        if (((char*)boundary)[i] == '\0') {
            printf("MEMORY BUG: %s:%d: detected wild write during free of pointer %p\n", file, line, ptr);
            abort();
        }
    }
}

void print_containing_block(void *ptr) {
    void *cur = latest_ptr;
    int min_dist = 0;
    size_t size = 0;

    while(cur) {
        int dist_from_block = ptr - cur;
        if (!min_dist || dist_from_block < min_dist) {
            min_dist = dist_from_block;
            size = get_size(cur);
        }

        if (!get_prev(cur))
            break;
        else
            cur = get_prev(cur);
    }

    printf("  %s:%d: %p is %d bytes inside a %zd byte region allocated here\n",
            get_filename(cur), get_line(cur), ptr, min_dist, size);
}

int removed(void *ptr) {
    if (ptr != NULL) {
        void *prev = get_prev(ptr);
        if (prev != NULL) {
            void *next = get_next(prev);
            return next != ptr;
        }
    }

    return 0;
}

void invalid_free_detect(void *ptr, const char *file, int line) {
    if(out_of_bounds(ptr)) {
        printf("MEMORY BUG: %s:%d: invalid free of pointer %p, not in heap\n", file, line, ptr);
        abort();
    }
    else if (!is_allocated(ptr) || removed(ptr)) {
        if (get_cur(ptr) == ptr) {
            printf("MEMORY BUG: %s:%d: double free of pointer %p\n", file, line, ptr);
            printf("  %s:%d: pointer %p previously freed here\n", get_filename(ptr), get_line(ptr), ptr);
            abort();
        }
        else {
            printf("MEMORY BUG: %s:%d: invalid free of pointer %p, not allocated\n", file, line, ptr);
            print_containing_block(ptr);
            abort();
        }
    }
}


void m61_free(void *ptr, const char *file, int line) {
    // avoid uninitialized variable warnings
    (void) file, (void) line;
    
    invalid_free_detect(ptr, file, line);
    boundary_write_detect(ptr, file, line);
    
    cs61_stats.active_count--;
    cs61_stats.active_size -= get_size(ptr);

    void *prev = get_prev(ptr);
    if (prev != NULL)
        set_next(prev, get_next(ptr));

    void *next = get_next(ptr);
    if (next != NULL)
        set_prev(next, prev);

    if (latest_ptr == ptr)
        latest_ptr = prev;

    alloc_info a;

    memcpy(&a, ((alloc_info*)ptr) - 1, sizeof(alloc_info));

    a.allocated = 0;
    a.filename = (char*)file;
    a.line = line;

    memcpy(((alloc_info*)ptr) - 1, &a, sizeof(alloc_info));

    free(((alloc_info*) ptr) - 1);
}

void *m61_realloc(void *ptr, size_t sz, const char *file, int line) {
    // avoid uninitialized variable warnings
    (void) file, (void) line;

    void *new_ptr = NULL;

    if (sz != 0)
        new_ptr = m61_malloc(sz, file, line);

    if (ptr != NULL && new_ptr != NULL) {
        size_t old_sz = get_size(ptr);
        if (sz > old_sz)
            memcpy(new_ptr, ptr, old_sz);
        else
            memcpy(new_ptr, ptr, sz);
    }

    if (ptr != NULL)
        m61_free(ptr, file, line);

    return new_ptr;
}

int will_overflow(unsigned int nmemb, unsigned int sz) {
    unsigned int total_size = nmemb * sz;
    return (total_size < sz || total_size < nmemb) && total_size;
}

void *m61_calloc(size_t nmemb, size_t sz, const char *file, int line) {
    // avoid uninitialized variable warnings
    (void) file, (void) line;
    
    if (will_overflow(nmemb, sz)) {
        cs61_stats.fail_count++;
        cs61_stats.fail_size += 0;
        return NULL;
    }

    void *ptr = m61_malloc(nmemb*sz, file, line);

    if (ptr != NULL)
        memset(ptr, 0, sz * nmemb);

    return ptr;
}

void m61_getstatistics(struct m61_statistics *stats) {
    // Stub: set all statistics to 0
    memset(stats, 0, sizeof(struct m61_statistics));
    memcpy(stats, &cs61_stats, sizeof(struct m61_statistics));
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
    void *cur = latest_ptr;

    while(cur != NULL) {
        printf("LEAK CHECK: %s:%d: allocated object %p with size %zd\n",
                get_filename(cur), get_line(cur), cur, get_size(cur));
        cur = get_prev(cur);
    }
}
