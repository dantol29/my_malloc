#include <string.h>
#include "ft_malloc.h"

void test_zones()
{
    void *ptr = malloc(16);
    memset(ptr, 'A', 16);

    void *ptr2 = malloc(2);
    memset(ptr2, 'B', 2);

    void *ptr3 = malloc(9);
    memset(ptr3, 'C', 9);

    free(ptr2);

    void *ptr4 = malloc(20);
    memset(ptr4, 'D', 20);

    void *ptr5 = malloc(2); // should be placed where ptr2 was
    memset(ptr5, 'E', 2);

    void *ptr6 = malloc(24); // next tiny zone
    memset(ptr6, 'F', 24);

    void *ptr7 = malloc(19);
    memset(ptr7, 'G', 19);

    void *ptr8 = malloc(129); // small zone
    memset(ptr8, 'A', 129);

    void *ptr9 = malloc(300000); // large zone
    memset(ptr9, 'B', 300000);
}

void test_realloc()
{
    void *r1 = malloc(16);
    memset(r1, 'A', 16);

    void *r2 = realloc(r1, 16); // same size

    void *r3 = realloc(r2, 17); // a byte bigger

    void *r4 = realloc(r3, 15); // a byte smaller

    void *r5 = realloc(r4, 2); // a lot smaller

    void *r6 = realloc(r5, 30); // a lot bigger

    void *r7 = realloc(r6, 24); // should take r1 place
}

// void test_coalescing()

// TODO:
// 1. fix copy in realloc
// 2. fix coalescing
// 3. add unmmap in free
int main()
{
    // test_coalescing();
    test_realloc();
    test_zones();
    return 0;
}
