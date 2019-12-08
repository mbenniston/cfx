#ifndef CFX_MEMTEST_H
#define CFX_MEMTEST_H
#include <stdlib.h>
#include <stdio.h>

#ifndef NDEBUG 

extern int cfx_numMallocs, cfx_numFrees;

static void* cfx_malloc(size_t bytes, const char* file, int line) {
    void* p = malloc(bytes);
    cfx_numMallocs++;
    printf("malloc'ed in %s:%d\n", file, line);
    return p;
}

static void cfx_free(void* d, const char* file, int line) {
    free(d);
    cfx_numFrees++;
    printf("free'ed in %s:%d\n", file, line);
}

#define malloc(x) cfx_malloc(x, __FILE__, __LINE__)
#define free(x) cfx_free(x, __FILE__, __LINE__)

#endif

#endif