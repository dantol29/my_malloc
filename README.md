# Custom Malloc Implementation

## Malloc's Heap (Tiny and Small zones)

![Zone scheme](/zone.png)

## Run tests

### Linux

`make re && gcc -g tests/test_complex.c -o test -L. -lft_malloc -Wl,-rpath=. && ./test`

### Mac

`make re && gcc -g tests/test_complex.c -o test -L. -lft_malloc  && ./test`

## Used functions

1. mmap - request memory from os
2. munmap - return memory to os

## Features

1. Coalescing
2. Arenas for small and tiny allocations
3. Explicit free list for O(free blocks) search time

## The memory translation process

_VIRTUAL MEMORY_ -> _PHYSICAL MEMORY_

- **TLB Lookup** - TLB is a cache that stores recent mappings between virtual addresses and physical addresses.
  When a virtual address needs translation, the MMU(Memory Mgmt Unit) first checks TLB.

- **TLB Hit**: If the mapping is found in the TLB, the physical address is returned.
- **TLB Miss**: If the mapping is not found in the TLB, the MMU performs a page walk to look up the mapping in the page table.

  - **Page Table Results**
    - Mapping exists: The MMU writes the mapping into the TLB so that future translations result in a TLB Hit.
    - Mapping does not exist: A page fault occurs. If address is invalid **segmentation fault** is triggered. If address is not loaded in RAM, it is stored on disk. OS retrieves the data from disk and updates page table.

## Materials

- CS361: Systems Programming, University Of Illinois Chicago
