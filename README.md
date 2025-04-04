## Used functions
1. mmap - request memory from os
2. munmap
3. getpagesize
4. getrlimit

## The translation process
__https://en.wikipedia.org/wiki/Page_table#/media/File:Page_table_actions.svg__
1. TLB Lookup - TLB is a small, fast cache located in the Memory Mgmt Unit. It stores recent mappings between virtual addresses and physical addresses.
When a virtual address needs translation, the MMU first check the TLB to see if the mapping is already stored there.

TLB Hit: If the mapping is found in the TLB, the physical address is returned.

2. TLB Miss and Page Table Lookup

TLB Miss: If the mapping is not found in the TLB, the MMU performs a page walk to look up the mapping in the page table.
The page table contains all mappings between virtual and physical adresses.

3. Handling Page Table Results
Mapping exists: if a valid mapping exists in the page table, it is returned to the MMU. The MMU writes this mapping back into the TLB so that future translations result in a TLB Hit.

Mapping does not exist: If no valid mapping exists in the page table, a page fault occurs for 2 reasons.
	1. Virtual address is invalid. It will cause a segmentation fault.
	2. virtual address is not loaded in RAM. If the the virtual address corresponds to a valid page but is stored on disk, the OS retrieves the data from secondary storage(disk or SSD):

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
