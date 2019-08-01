#include "ncx_slab.h"
#include <sys/time.h>

uint64_t current_microseconds() {
    struct timeval tv;
    uint64_t usec;

    gettimeofday(&tv, NULL);

    usec = ((uint64_t) tv.tv_sec) * 1000000LL;
    usec += tv.tv_usec;

    return usec;
}

int run_bench(int argc, char **argv) {
    (void) argc;
    (void) argv;
    printf("current machine ptr_size=%ld\n", sizeof(argv));

    ncx_slab_pool_t *sp;
    ncx_slab_stat_t stat;
    size_t pool_size;
    ncx_u_char *space;

    pool_size = 4096000;  //4M
    space = (ncx_u_char *) malloc(pool_size);
    sp = (ncx_slab_pool_t *) space;

    sp->addr = space;
    sp->min_shift = 3;
    sp->end = space + pool_size;

    ncx_slab_init(sp);

    printf("before run bench, ncx_slab_stat:\n");
    ncx_slab_stat(sp, &stat);

    printf("\n=================Now Run Bench=================\n");

    char *p;
    uint64_t microseconds_begin;
    uint64_t microseconds_end;
    size_t size[] = {30, 120, 256, 500, 1000, 2000, 3000, 5000, 256000, 512000, 1024000, 3072000};

    printf("size\tncx\tmalloc\tpct\n");

    uint64_t i, j;
    uint64_t t1, t2;
    //#define RUNNING_TIMES 1000000
#define RUNNING_TIMES 100000
    for (j = 0; j < sizeof(size) / sizeof(size_t); j++) {
        size_t current_test_alloc_size = size[j];
        printf("%ld\t", current_test_alloc_size);

        //test for ncx_pool
        microseconds_begin = current_microseconds();
        for (i = 0; i < RUNNING_TIMES; i++) {
            p = ncx_slab_alloc(sp, current_test_alloc_size);
            if (p) {
                p[0] = '0';
//                strcpy(p, "Hello World");
                ncx_slab_free(sp, p);
            } else {
                printf("ncx_slab_alloc %ld failed\n", current_test_alloc_size);
                return -1;
            }
        }
        microseconds_end = current_microseconds();

        t1 = (microseconds_end - microseconds_begin);
        printf("%lu \t", t1 / 1000);

        // test for malloc
        microseconds_begin = current_microseconds();
        for (i = 0; i < RUNNING_TIMES; i++) {
            p = (char *) malloc(current_test_alloc_size);
            if (p) {
                //do sth with p, or compiler may optimize your code(omit it).
                p[0] = '0';
//                strcpy(p, "Hello World");
                free(p);
            } else {
                printf("malloc %ld failed\n", current_test_alloc_size);
                return -1;
            }
        }
        microseconds_end = current_microseconds();

        t2 = (microseconds_end - microseconds_begin);
        if (t2 == 0) {
//            printf("malloc is too fast. t2==0\n");
            t2 = 1000;// safe belt: make sure we won't divide zero at below.
        }
        printf("%lu\t", t2 / 1000);

        printf("%.2f\n", (double) t1 / (double) t2);
    }

    printf("\n=================All bench tests executed!=================\n");
    ncx_slab_stat(sp, &stat);

    free(space);

    return 0;
}
