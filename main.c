#include "ncx_slab.h"
#include <assert.h>

#define UNUSED_PARAMETER(var) (void)var;
#define DEF_TEST(name) \
    static int test_##name()

#define RUN_TEST(func_name) \
    do { printf("RUN_TEST: %s ->\n", #func_name); assert(func_name() == 0); printf("******* %s  [  OK  ] *******\n\n\n\n", #func_name); } while(0);

extern uint64_t current_microseconds();

extern int run_bench(int argc, char **argv);

DEF_TEST(benchmark) {
    return run_bench(0, NULL);
}

DEF_TEST(basic) {
    char *p;
    size_t pool_size = 4096000;  //4M
    ncx_slab_stat_t stat;
    u_char *space;
    space = (u_char *) malloc(pool_size);

    ncx_slab_pool_t *sp;
    sp = (ncx_slab_pool_t *) space;

    sp->addr = space;
    sp->min_shift = 3;
    sp->end = space + pool_size;

    ncx_slab_init(sp);

    size_t i;
    for (i = 0; i < 1000000; i++) {
        p = ncx_slab_alloc(sp, 128 + i);
        if (p == NULL) {
            printf("alloc %ld failed at %ld\n", 128 + i, i);
            return -1;
        }
        //	printf("main for i=%d main p=%u\n",i,(uintptr_t)p);
        ncx_slab_free(sp, p);
    }
    ncx_slab_stat(sp, &stat);

    printf("\n##########################################################################\n");
    const size_t unfreed_ptrs_array_capacity = 2500 - 2500 / 3;
    uintptr_t unfreed_ptrs[unfreed_ptrs_array_capacity];
    memset(unfreed_ptrs, 0, unfreed_ptrs_array_capacity * sizeof(uintptr_t));
    size_t unfreed_ptrs_index = 0;
    for (i = 0; i < 2500; i++) {
        p = ncx_slab_alloc(sp, 30 + i);
        if (p == NULL) {
            printf("alloc %ld failed at %ld\n", 30 + i, i);
            return -1;
        }

        if (i % 3 == 0) {
            ncx_slab_free(sp, p);
        } else {
            unfreed_ptrs[unfreed_ptrs_index++] = (uintptr_t)p;
        }
    }
    printf("after misc malloc/free, current status:\n");
    ncx_slab_stat(sp, &stat);
    for (i = 0; i < unfreed_ptrs_array_capacity; i++) {
        p = (char *)unfreed_ptrs[i];
        if (p != NULL) {
            ncx_slab_free(sp, p);
        }
    }
    printf("after free all, status:\n");
    ncx_slab_stat(sp, &stat);

    free(space);
    return 0;
}

int main(int argc, char **argv) {
    UNUSED_PARAMETER(argc)
    UNUSED_PARAMETER(argv)

//    RUN_TEST(test_basic)
    RUN_TEST(test_benchmark)

    printf("\n\n\n\n***** ALL TESTS PASSED!!! *****");
    return 0;
}
