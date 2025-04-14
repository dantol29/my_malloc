#include <stdio.h>
// #include <stdlib.h>
#include <time.h>
#include "../includes/ft_malloc.h"

#define NUM_ALLOCATIONS 10000

void benchmark_malloc()
{
    void *pointers[NUM_ALLOCATIONS];
    clock_t start, end;

    // Measure allocation speed
    start = clock();
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        pointers[i] = malloc(50);
    }
    end = clock();
    printf("Allocation time: %lf ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);

    // Measure deallocation speed
    start = clock();
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        free(pointers[i]);
    }
    end = clock();
    printf("Deallocation time: %lf ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);
}

int main()
{
    benchmark_malloc();
    return 0;
}
