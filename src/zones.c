#include "../includes/ft_malloc_internal.h"

void initialize_zone(const size_t aligned_size)
{
    if (aligned_size <= TINY)
    {
        if (heap.tiny_zone == NULL)
            heap.tiny_zone = allocate_zone(TINY_ZONE_SIZE, NULL, NULL, &heap.tiny_free_list_head);
    }
    else if (aligned_size <= SMALL)
    {
        if (heap.small_zone == NULL)
            heap.small_zone = allocate_zone(SMALL_ZONE_SIZE, NULL, NULL, &heap.small_free_list_head);
    }
    else
    {
        if (heap.large_zone == NULL)
            heap.large_zone = allocate_zone(aligned_size, NULL, NULL, NULL);
    }
}

inline void *get_zone(const size_t aligned_size)
{
    if (aligned_size <= TINY)
        return heap.tiny_zone;

    if (aligned_size <= SMALL)
        return heap.small_zone;
    else
        return heap.large_zone;
}

inline int get_zone_size(const size_t aligned_size)
{
    if (aligned_size <= TINY)
        return TINY_ZONE_SIZE;
    if (aligned_size <= SMALL)
        return SMALL_ZONE_SIZE;

    return 0;
}

void *get_last_zone(const size_t size)
{
    struct s_zone *zone = get_zone(size);
    if (!zone)
        return NULL;

    while (zone->next)
        zone = zone->next;

    return zone;
}

void *allocate_zone(const size_t zone_size, struct s_zone *prev, void *prev_free_block, void **head)
{
    struct s_zone *zone = mmap(0, sizeof(void *) * 2 + sizeof(size_t) + zone_size + sizeof(size_t), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (zone == MAP_FAILED)
        return NULL;

    zone->next = NULL;
    zone->prev = prev;

    if (prev)
        prev->next = zone;

    *(size_t *)((char *)zone + METADATA_SIZE) = zone_size; // skip 2 pointers and start boundary to set size

    // set ptrs for explicit free list
    *(uintptr_t *)((char *)zone + METADATA_SIZE + NEXT) = 0;
    *(uintptr_t *)((char *)zone + METADATA_SIZE + PREV) = (uintptr_t)prev_free_block;

    if (head && !*head)
        *head = (char *)zone + METADATA_SIZE;

    return zone;
}
