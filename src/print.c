#include "../includes/ft_malloc_internal.h"

static void safe_write(int fd, const void *buf, size_t count) {
    ssize_t ret = write(fd, buf, count);
    if (ret < 0) {
        (void)ret;
    }
}

inline static void	ft_putchar(char c)
{
	safe_write(1, &c, 1);
}


static void write_size_t(size_t value)
{
    char buf[32];
    int i = 31;

    buf[i--] = '\0';

    if (value == 0)
    {
        buf[i--] = '0';
    }
    else
    {
        while (value > 0 && i >= 0)
        {
            buf[i--] = '0' + (value % 10);
            value /= 10;
        }
    }

    safe_write(1, &buf[i + 1], 31 - i);
}

static void	ft_put_ptr(uintptr_t num)
{
	if (num >= 16)
	{
		ft_put_ptr(num / 16);
		ft_put_ptr(num % 16);
	}
	else
	{
		if (num <= 9)
			ft_putchar(num + '0');
		else
			ft_putchar(num - 10 + 'a');
	}
}

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
		ft_put_ptr((uintptr_t)((char *)zone + METADATA_SIZE));
		safe_write(1, " - ", 4);
		ft_put_ptr((uintptr_t)((char *)current_block + block_size));
                safe_write(1, " : ", 4);
                write_size_t(zone_bytes_allocated);
		safe_write(1, " bytes\n", 8);
                
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
	safe_write(1, "TINY: 0x", 9);
	ft_put_ptr((uintptr_t)heap.tiny_zone);
	safe_write(1, "\n", 2);
        total_bytes += print_zone(heap.tiny_zone, TINY_ZONE_SIZE);
    }

    if (heap.small_zone)
    {
	safe_write(1, "SMALL: 0x", 10);
        ft_put_ptr((uintptr_t)heap.small_zone);
        safe_write(1, "\n", 2);
	total_bytes += print_zone(heap.small_zone, SMALL_ZONE_SIZE);
    }

    /*if (heap.large_zone)
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
    }*/

    //ft_printf("Total: %u\n", total_bytes);
}
