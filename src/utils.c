#include "../includes/ft_malloc_internal.h"

void *get_zone(const size_t aligned_size)
{
        if (aligned_size <= TINY)
        {
                if (heap.tiny_zone == NULL)
                        heap.tiny_zone = allocate_zone(TINY_ZONE_SIZE, NULL);
                return heap.tiny_zone;
        }

        if (aligned_size <= SMALL)
        {
                if (heap.small_zone == NULL)
                        heap.small_zone = allocate_zone(SMALL_ZONE_SIZE, NULL);
                return heap.small_zone;
        }
        else
        {
                if (heap.large_zone == NULL)
                        heap.large_zone = allocate_zone(aligned_size, NULL);
                return heap.large_zone;
        }
}

int get_zone_size(const size_t aligned_size)
{
        if (aligned_size <= TINY)
                return TINY_ZONE_SIZE;
        if (aligned_size <= SMALL)
                return SMALL_ZONE_SIZE;

        return 0;
}

void *allocate_zone(const size_t zone_size, struct s_zone *prev)
{
        struct s_zone *zone = mmap(0, sizeof(void *) * 2 + sizeof(size_t) + zone_size + sizeof(size_t), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (zone == MAP_FAILED)
                return NULL;

        zone->next = NULL;
        zone->prev = prev;

        if (prev)
                prev->next = zone;

        *(size_t *)((char *)zone + METADATA_SIZE) = zone_size; // skip 2 pointers and start boundary to set size

        return zone;
}

void *ft_memcpy(void *dest, const void *src, size_t n)
{
        size_t i;
        unsigned char *d;
        const unsigned char *s;

        d = (unsigned char *)dest;
        s = (unsigned char *)src;
        i = 0;
        while (i < n)
        {
                d[i] = s[i];
                ++i;
        }
        return dest;
}

inline void align_size(size_t *size)
{
        if (!size)
                return;

        *size = *size + sizeof(size_t) * 2;                 // 2 fields to store header and footer
        *size = (*size + (ALIGNING - 1)) & ~(ALIGNING - 1); // align to the nearest multiplier of ALIGNING (16 -> 16,32,48)
}
