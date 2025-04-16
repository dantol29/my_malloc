#include "../includes/ft_malloc.h"

void print_zone(struct s_zone *zone, size_t zone_size)
{
    while (zone)
    {
        void *current_block = (void *)((char *)zone + METADATA_SIZE);
        size_t tmp = 0;
        size_t count = 0;
        size_t taken_bytes = 0;

        while (current_block)
        {
            tmp = *(size_t *)current_block;
            if (*(size_t *)current_block & 1)
            {
                tmp -= 1;
                taken_bytes += tmp;
            }
            // ft_printf("block: %p, ", current_block);
            // ft_printf("taken_butes: %u, ", taken_bytes);
            // ft_printf("tmp: %u\n", tmp);
            count += tmp;
            if (count >= zone_size)
            {
                ft_printf("%p - ", (char *)zone + METADATA_SIZE);
                ft_printf("%p : ", (char *)current_block + tmp);
                ft_printf("%u bytes\n", taken_bytes);
                break;
            }

            current_block = (char *)current_block + tmp;
        }

        zone = zone->next;
    }
}

void show_alloc_mem()
{
    if (heap.tiny_zone)
    {
        ft_printf("TINY: %p\n", heap.tiny_zone);
        print_zone(heap.tiny_zone, TINY_ZONE_SIZE);
    }

    if (heap.small_zone)
    {
        ft_printf("SMALL: %p\n", heap.small_zone);
        print_zone(heap.small_zone, SMALL_ZONE_SIZE);
    }

    if (heap.large_zone)
    {
        ft_printf("LARGE: %p\n", heap.large_zone);
        struct s_zone *current_zone = heap.large_zone;

        while (current_zone)
        {
            size_t size = *(size_t *)((char *)current_zone + METADATA_SIZE);
            ft_printf("%p - ", (char *)current_zone + METADATA_SIZE);
            ft_printf("%p : ", (char *)current_zone + size);
            ft_printf("%u bytes\n", size);
            current_zone = current_zone->next;
        }
    }
}
