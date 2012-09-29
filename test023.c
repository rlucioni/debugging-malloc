#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// test023: memory leak report, multiple leaks

struct node {
    struct node *next;
};
typedef struct node node;

int main() {
    char *ptrs[10];
    ptrs[0] = (char *) malloc(10);
    ptrs[1] = (char *) malloc(11);
    ptrs[2] = (char *) malloc(12);
    ptrs[3] = (char *) malloc(13);
    ptrs[4] = (char *) malloc(14);
    ptrs[5] = (char *) malloc(15);
    ptrs[6] = (char *) malloc(16);
    ptrs[7] = (char *) malloc(17);
    ptrs[8] = (char *) malloc(18);
    ptrs[9] = (char *) malloc(19);

    free(ptrs[3]);
    free(ptrs[8]);
    free(ptrs[0]);
    m61_printleakreport();
}

// The "//!!SORT" line tells the compare.pl script to sort your program's
// output before comparing it with the list below.
// That's so your program can output its reports in any order.

//!!SORT
//! LEAK CHECK: test023.c:15: allocated object ??{\w+}?? with size 11
//! LEAK CHECK: test023.c:16: allocated object ??{\w+}?? with size 12
//! LEAK CHECK: test023.c:18: allocated object ??{\w+}?? with size 14
//! LEAK CHECK: test023.c:19: allocated object ??{\w+}?? with size 15
//! LEAK CHECK: test023.c:20: allocated object ??{\w+}?? with size 16
//! LEAK CHECK: test023.c:21: allocated object ??{\w+}?? with size 17
//! LEAK CHECK: test023.c:23: allocated object ??{\w+}?? with size 19
