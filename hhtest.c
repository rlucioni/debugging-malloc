#include "m61.h"
#include <stdlib.h>
#include <math.h>
#define NALLOCATORS 40
// hhtest: A sample framework for evaluating heavy hitter reports.

// 40 different allocation functions give 40 different call sites
void f00(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f01(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f02(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f03(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f04(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f05(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f06(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f07(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f08(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f09(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f10(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f11(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f12(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f13(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f14(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f15(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f16(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f17(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f18(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f19(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f20(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f21(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f22(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f23(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f24(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f25(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f26(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f27(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f28(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f29(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f30(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f31(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f32(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f33(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f34(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f35(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f36(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f37(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f38(size_t sz) { void *ptr = malloc(sz); free(ptr); }
void f39(size_t sz) { void *ptr = malloc(sz); free(ptr); }

// An array of those allocation functions
void (*allocators[])(size_t) = {
    &f00, &f01, &f02, &f03, &f04, &f05, &f06, &f07, &f08, &f09,
    &f10, &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
    &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28, &f29,
    &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37, &f38, &f39
};

// Sizes passed to those allocation functions.
// Later allocation functions have much bigger sizes.
size_t sizes[NALLOCATORS] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 4, 8, 16, 32, 64,
    128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536
};

int main(int argc, char **argv) {
    double skew = 0;
    unsigned long long count = 1000000;
    if (argc >= 2)
	skew = strtod(argv[1], 0);
    if (argc >= 3)
	count = strtoull(argv[2], 0, 0);

    // Calculate the probability we'll call allocator I.
    // That probability equals  2^(-I*skew) / \sum_{i=0}^40 2^(-I*skew).
    // When skew=0, every allocator is called with equal probability.
    // When skew=1, the first allocator is called twice as often as the second,
    //   which is called twice as often as the third, and so forth.
    // When skew=-1, the first allocator is called HALF as often as the second,
    //   which is called HALF as often as the third, and so forth.
    double sum_p = 0;
    for (int i = 0; i < NALLOCATORS; ++i)
	sum_p += pow(0.5, i * skew);
    long limit[NALLOCATORS];
    double ppos = 0;
    for (int i = 0; i < NALLOCATORS; ++i) {
	ppos += pow(0.5, i * skew);
	limit[i] = RAND_MAX * (ppos / sum_p);
    }
    // Now the probability we call allocator I equals
    // (limit[i] - limit[i-1]) / (double) RAND_MAX,
    // if we pretend that limit[-1] == 0.

    // Pick `count` random allocators and call them.
    for (unsigned long long i = 0; i < count; ++i) {
	long x = random();
	int r = 0;
	while (r < NALLOCATORS - 1 && x > limit[r])
	    ++r;
	allocators[r](sizes[r]);
    }
}
