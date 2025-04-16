#include <time.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
// #include <stdlib.h>
#include "../includes/ft_malloc.h"

int is_memory_writable(void *ptr, size_t size)
{
    if (ptr == NULL)
        return 0;

    unsigned char *p = (unsigned char *)ptr;
    for (size_t i = 0; i < size; i++)
    {
        p[i] = 0xAA;
        if (p[i] != 0xAA)
            return 0;
    }
    return 1;
}

void test_basic_malloc()
{
    printf("Testing basic malloc...\n");

    // Simple allocation test
    void *ptr = malloc(20);
    assert(ptr != NULL);

    // Write to memory to ensure it's usable
    memset(ptr, 0x67, 20);

    // Check if memory was properly written
    unsigned char *p = (unsigned char *)ptr;
    for (int i = 0; i < 20; i++)
    {
        assert(p[i] == 0x67);
    }

    free(ptr);
    printf("Basic malloc test passed\n");
}

void test_malloc_zero()
{
    printf("Testing malloc with zero size...\n");

    // Malloc should handle zero size appropriately
    // (either return NULL or a small valid pointer)
    void *ptr = malloc(0);

    // If it returns a pointer, we should be able to free it
    if (ptr != NULL)
    {
        free(ptr);
    }

    printf("Malloc zero test passed\n");
}

void test_multiple_allocations()
{
    printf("Testing multiple allocations...\n");

    // Allocate multiple blocks of different sizes
    void *ptrs[100];
    for (int i = 0; i < 100; i++)
    {
        size_t size = (i + 1) * 10;
        ptrs[i] = malloc(size);
        assert(ptrs[i] != NULL);

        // Write to memory to ensure it's usable
        memset(ptrs[i], i % 256, size);
    }

    // Verify each block contains the expected data
    for (int i = 0; i < 100; i++)
    {
        size_t size = (i + 1) * 10;
        unsigned char *p = (unsigned char *)ptrs[i];
        for (size_t j = 0; j < size; j++)
        {
            assert(p[j] == i % 256);
        }
    }

    // Free all blocks
    for (int i = 0; i < 100; i++)
    {
        free(ptrs[i]);
    }

    printf("Multiple allocations test passed\n");
}

void test_free_and_reuse()
{
    printf("Testing free and memory reuse...\n");

    // Allocate a block
    void *ptr1 = malloc(100);
    assert(ptr1 != NULL);

    // Store the address
    uintptr_t addr1 = (uintptr_t)ptr1;

    // Free the block
    free(ptr1);

    // Allocate again with same size, might reuse the freed memory
    void *ptr2 = malloc(100);
    assert(ptr2 != NULL);

    // Print whether memory was reused (informational only)
    if ((uintptr_t)ptr2 == addr1)
    {
        printf("Memory was reused (good!)\n");
    }
    else
    {
        printf("Memory was not reused (might be normal depending on implementation)\n");
    }

    free(ptr2);
    printf("Free and reuse test passed\n");
}

void test_memory_alignment()
{
    printf("Testing memory alignment...\n");

    // Test that allocated memory is properly aligned
    void *ptr = malloc(1);
    assert(ptr != NULL);

    // Check alignment (assuming 16-byte alignment)
    uintptr_t addr = (uintptr_t)ptr;
    assert((addr & 0xF) == 0);

    free(ptr);
    printf("Memory alignment test passed\n");
}

void test_large_allocation()
{
    printf("Testing large allocation...\n");

    // Try to allocate a large block
    void *ptr = malloc(1024 * 1024); // 1MB
    assert(ptr != NULL);

    // Write to memory to ensure it's usable
    memset(ptr, 0x42, 1024 * 1024);

    // free(ptr);
    printf("Large allocation test passed\n");
}

void test_stress()
{
    printf("Running stress test...\n");

    // Perform many allocations and frees
    void *ptrs[1000];

    // Allocate 1000 blocks
    for (int i = 0; i < 1000; i++)
    {
        ptrs[i] = malloc((i % 100) + 1);
        assert(ptrs[i] != NULL);
    }

    // Free every other block
    for (int i = 0; i < 1000; i += 2)
    {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }

    // Allocate new blocks
    for (int i = 0; i < 1000; i += 2)
    {
        ptrs[i] = malloc(((i % 100) + 1) * 2);
        assert(ptrs[i] != NULL);
    }

    // Free all blocks
    for (int i = 0; i < 1000; i++)
    {
        if (ptrs[i] != NULL)
        {
            free(ptrs[i]);
        }
    }

    printf("Stress test passed\n");
}

void test_malloc_free_cycle()
{
    printf("Testing malloc-free cycles...\n");

    // Perform many malloc-free cycles
    for (int i = 0; i < 1000; i++)
    {
        void *ptr = malloc(i % 200 + 1);
        assert(ptr != NULL);

        // Write some data
        memset(ptr, 0x55, i % 200 + 1);

        free(ptr);
    }

    printf("Malloc-free cycle test passed\n");
}

void test_fragmentation()
{
    printf("Testing memory fragmentation handling...\n");

#define NUM_BLOCKS 100
    void *blocks[NUM_BLOCKS];

    // Allocate blocks of increasing size
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        blocks[i] = malloc((i + 1) * 16);
        assert(blocks[i] != NULL);
    }

    // Free every other block to create fragmentation
    for (int i = 0; i < NUM_BLOCKS; i += 2)
    {
        free(blocks[i]);
        blocks[i] = NULL;
    }

    // Now allocate blocks that should fit in the holes
    for (int i = 0; i < NUM_BLOCKS; i += 2)
    {
        blocks[i] = malloc((i + 1) * 8); // Smaller than original
        assert(blocks[i] != NULL);
    }

    // Free all blocks
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (blocks[i] != NULL)
        {
            free(blocks[i]);
        }
    }

    printf("Fragmentation test passed\n");
}

void test_boundary_conditions()
{
    printf("Testing boundary conditions...\n");

    // Test very small allocations
    void *ptr1 = malloc(1);
    assert(ptr1 != NULL);

    // Test odd-sized allocations
    void *ptr2 = malloc(3);
    assert(ptr2 != NULL);
    void *ptr3 = malloc(7);
    assert(ptr3 != NULL);

    // Test allocation sizes near power-of-two boundaries
    void *ptr4 = malloc(31);
    assert(ptr4 != NULL);
    void *ptr5 = malloc(32);
    assert(ptr5 != NULL);
    void *ptr6 = malloc(33);
    assert(ptr6 != NULL);
    void *ptr7 = malloc(34);
    assert(ptr7 != NULL);

    // Free all allocations
    // free(ptr1);
    // free(ptr2);
    // free(ptr3);
    // free(ptr4);
    // free(ptr5);
    // free(ptr6);

    printf("Boundary conditions test passed\n");
}

void test_malloc_performance()
{
    printf("Testing malloc performance...\n");

#define PERF_ITERATIONS 10000
#define ALLOC_SIZE 128

    clock_t start = clock();

    for (int i = 0; i < PERF_ITERATIONS; i++)
    {
        void *ptr = malloc(ALLOC_SIZE);
        assert(ptr != NULL);
        free(ptr);
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Performed %d malloc/free cycles in %.3f seconds\n",
           PERF_ITERATIONS, time_spent);
    printf("Average time per cycle: %.9f seconds\n",
           time_spent / PERF_ITERATIONS);

    printf("Performance test completed\n");
}

void run_all_tests()
{
    printf("Running malloc and free tests...\n");

    test_basic_malloc();
    test_malloc_zero();
    test_multiple_allocations();
    test_free_and_reuse();
    test_memory_alignment();
    test_large_allocation();
    test_stress();
    test_malloc_free_cycle();
    test_fragmentation();
    test_boundary_conditions();
    test_malloc_performance();
    printf("All tests passed!!\n");

    show_alloc_mem();
}

int main()
{
    run_all_tests();
    return 0;
}
