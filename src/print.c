#include "../includes/ft_malloc_internal.h"

static size_t print_zone(struct s_zone *zone, size_t zone_size)
{
    size_t total_bytes_allocated = 0;

    while (zone)
    {
        void *current_block = (void *)((char *)zone + METADATA_SIZE);
        size_t block_size = 0;
        size_t count = 0;
        size_t zone_bytes_allocated = 0;

        while (current_block)
        {
            block_size = *(size_t *)current_block;
            if (*(size_t *)current_block & 1)
            {
                block_size -= 1;
                zone_bytes_allocated += block_size;
            }

            count += block_size;
            if (count >= zone_size)
            {
                ft_printf("%p - ", (char *)zone + METADATA_SIZE);
                ft_printf("%p : ", (char *)current_block + block_size);
                ft_printf("%u bytes\n", zone_bytes_allocated);
                total_bytes_allocated += zone_bytes_allocated;
                break;
            }

            current_block = (char *)current_block + block_size;
        }

        zone = zone->next;
    }

    return total_bytes_allocated;
}

void show_alloc_mem() // TODO: double check correctness
{
    size_t total_bytes = 0;

    if (heap.tiny_zone)
    {
        ft_printf("TINY: %p\n", heap.tiny_zone);
        total_bytes += print_zone(heap.tiny_zone, TINY_ZONE_SIZE);
    }

    if (heap.small_zone)
    {
        ft_printf("SMALL: %p\n", heap.small_zone);
        total_bytes += print_zone(heap.small_zone, SMALL_ZONE_SIZE);
    }

    if (heap.large_zone)
    {
        ft_printf("LARGE: %p\n", heap.large_zone);
        struct s_zone *current_zone = heap.large_zone;

        while (current_zone)
        {
            size_t size = *(size_t *)((char *)current_zone + METADATA_SIZE) - 1; // last bit is allocated
            ft_printf("%p - ", (char *)current_zone + METADATA_SIZE);
            ft_printf("%p : ", (char *)current_zone + size);
            ft_printf("%u bytes\n", size);
            total_bytes += size;
            current_zone = current_zone->next;
        }
    }

    ft_printf("Total: %u\n", total_bytes);
}
