#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#define ALIGNING 16
#define TINY 256
#define TINY_ZONE_SIZE 25600   // 25600 25KB
#define SMALL 4096             // 4KB
#define SMALL_ZONE_SIZE 409600 // 400KB

#define METADATA_SIZE sizeof(void *) * 2 + sizeof(size_t) // 2 pointers + start boundary

#include <stddef.h>   // size_t
#include <sys/mman.h> // mmap
#include "../libprintf/ft_printf.h"

struct s_zone
{
        struct s_zone *next;
        struct s_zone *prev;
};

struct s_heap
{
        struct s_zone *tiny_zone;
        struct s_zone *small_zone;
        struct s_zone *large_zone;
};

extern struct s_heap heap;

// malloc.c
void *malloc(size_t size);
void show_alloc_mem();

// realloc.c
void *realloc(void *ptr, size_t size);

// free.c
void free(void *ptr);

// utils.c
void *get_zone(const size_t aligned_size);
int get_zone_size(const size_t aligned_size);
void *allocate_zone(const size_t zone_size, struct s_zone *prev);
void *ft_memcpy(void *dest, const void *src, size_t n);

#endif
