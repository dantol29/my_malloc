#ifndef FT_MALLOC_INTERNAL_H
#define FT_MALLOC_INTERNAL_H

#include <stddef.h>   // size_t
#include <sys/mman.h> // mmap
#include "../libprintf/ft_printf.h"

inline static size_t get_page_size()
{
#if defined(_SC_PAGESIZE)
    return (size_t)sysconf(_SC_PAGESIZE);
#else
    return 4096;
#endif
}

#define ALIGNING 16
#define TINY 256
#define SMALL get_page_size()
#define TINY_ZONE_SIZE get_page_size() * 6
#define SMALL_ZONE_SIZE get_page_size() * 100 // 400KB

#define METADATA_SIZE sizeof(void *) * 2 + sizeof(size_t) // 2 pointers + start boundary

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

#pragma GCC visibility push(hidden) // make functions hidden to library users

extern struct s_heap heap;

void *get_zone(const size_t aligned_size);
int get_zone_size(const size_t aligned_size);
void *allocate_zone(const size_t zone_size, struct s_zone *prev);
void *ft_memcpy(void *dest, const void *src, size_t n);
void align_size(size_t *size);

#pragma GCC visibility pop

#endif
