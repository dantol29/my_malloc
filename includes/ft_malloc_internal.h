#ifndef FT_MALLOC_INTERNAL_H
#define FT_MALLOC_INTERNAL_H

#include <sys/mman.h> // mmap
#include <unistd.h>
#include <stdint.h>

inline static size_t get_page_size()
{
#if defined(_SC_PAGESIZE)
    return (size_t)sysconf(_SC_PAGESIZE);
#else
    return 4096;
#endif
}

#define METADATA_SIZE sizeof(void *) * 2 + sizeof(size_t) // 2 pointers + start boundary

#define ALIGNING sizeof(void *) * 2 // 16
#define TINY 256
#define SMALL get_page_size()
#define TINY_ZONE_SIZE get_page_size() * 7 - METADATA_SIZE - sizeof(size_t)
#define SMALL_ZONE_SIZE get_page_size() * 101 - METADATA_SIZE - sizeof(size_t)
#define MIN_SPLIT_SIZE sizeof(size_t) * 4 // 2 pointers + header and footer
#define HEADER_SIZE sizeof(size_t)
// zone memory structure:
// void* next_zone, void* prev_zone, size_t start_boundary, (zone), size_t end_boundary
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
    void *tiny_free_list_head;
    void *small_free_list_head;
};

enum free_ptrs
{
    NEXT = sizeof(size_t),
    PREV = sizeof(size_t) * 2
};

#pragma GCC visibility push(hidden) // make functions hidden to library users

extern struct s_heap heap;

void *get_zone(const size_t aligned_size);
void *get_last_zone(const size_t size);
int get_zone_size(const size_t aligned_size);
void initialize_zone(const size_t aligned_size);
void *allocate_zone(const size_t zone_size, struct s_zone *prev, void *prev_free_block, void **head);

void push_to_free_list(void *free_block, void **head);
void remove_from_free_list(void *free_block, void **head, void *header);
void **get_free_list(const size_t size);
void print_free_list(void *head);

void *ft_memcpy(void *dest, const void *src, size_t n);
void align_size(size_t *size);

#pragma GCC visibility pop

#endif
