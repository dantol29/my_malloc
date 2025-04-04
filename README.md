## Used functions
1. mmap - request memory from os
2. munmap
3. getpagesize
4. getrlimit

## The memory translation process
_VIRTUAL MEMORY_ -> _PHYSICAL MEMORY_

1. **TLB Lookup** - TLB is a cache that stores recent mappings between virtual addresses and physical addresses.
When a virtual address needs translation, the MMU(Memory Mgmt Unit) first checks TLB.

	- **TLB Hit**: If the mapping is found in the TLB, the physical address is returned.
	- **TLB Miss**: If the mapping is not found in the TLB, the MMU performs a page walk to look up the mapping in the page table.

		- **Page Table Results**
			- Mapping exists: The MMU writes the mapping into the TLB so that future translations result in a TLB Hit.
			- Mapping does not exist: A page fault occurs. If address is invalid **segmentation fault** is triggered. If address is not loaded in RAM, it is stored on disk. OS retrieves the data from disk and updates page table.

## Page
a fixed-length block of virtual memory, described by a single entry in a page table. Computer memory is divided into pages so that information can be found more quickly.
- Smaller page size 
	- Drawbacks
	1. Uses more pages, requring a bigger page table. 
	2. TLB can hold mappings for fewer bytes
	3. Transfering many small pages between disk and memory is less efficient
	- Advantages
	1. Reduced internal fragmentation, minimizes wasted space
	2. Better cache utilization
	3. Better suited for apps with small memory needs, as they avoid over-allocationg unused memory
- Large page size
	- Drawbacks
	1. Require more memory to be allocated at once, which can lead to significant wasted space if proccesses do not use the allocated page
	2. Occupy a significant cache space even if only a small portion is actively used
	3. When a page fault occurs, loading a large page from disk takes more time
	- Advantages
	1. Smaller page table, saving memory
	2. Reduced TLB misses, since each TLB entry maps a larger portion of memory
	3. More data is written in a single I/O operation, which is faster than handling many small transfers
- Example:
	1. Page size is 1 GB:
		- A process requires 1.5 GB of memory.
		- Two pages are allocated (2 GB total), but only 1.5 GB is used.
		- The remaining 0.5 GB (half of the second page) is wasted due to internal fragmentation.
	2. Page size is 4 KB:
		- The proccess would require 1.5GB/4KB = 384_000 pages.
		- Wastage would be limited at most one partially filled 4KB page.
