#include "../includes/ft_malloc.h"

void *get_zone(const int aligned_size)
{
        if (aligned_size < TINY)
        {
                if (heap.tiny_zone == NULL)
                        heap.tiny_zone = allocate_zone(TINY_ZONE_SIZE, NULL);
                return heap.tiny_zone;
        }

        if (aligned_size < SMALL)
        {
                if (heap.small_zone == NULL)
                        heap.small_zone = allocate_zone(SMALL_ZONE_SIZE, NULL);
                return heap.small_zone;
        }
        else
        {
                if (heap.large_zone == NULL)
                        heap.large_zone = allocate_zone(aligned_size + sizeof(int), NULL);
                return heap.large_zone;
        }
}

int get_zone_size(const int aligned_size)
{
        if (aligned_size < TINY)
                return TINY_ZONE_SIZE;
        if (aligned_size < SMALL)
                return SMALL_ZONE_SIZE;

        return 0;
}

void *allocate_zone(const int zone_size, struct s_zone *prev)
{
        struct s_zone *zone = mmap(0, sizeof(struct s_zone) + zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (zone == MAP_FAILED)
                return NULL;

        zone->next = NULL;
        zone->prev = prev;

        if (prev)
                prev->next = zone;

        *(int *)((int *)zone + 4) = zone_size; // skip 2 pointers to set size

        return zone;
}
