#include "../includes/ft_malloc_internal.h"

struct s_heap heap = {0};

static void *create_large_allocation(const size_t aligned_size, struct s_zone *current_zone)
{
	if (!current_zone)
		return NULL;

	while (*((char *)current_zone + METADATA_SIZE) & 1)
	{
		if (!current_zone->next)
			current_zone = allocate_zone(aligned_size, current_zone);
		else
			current_zone = current_zone->next;
	}

	*(size_t *)((char *)current_zone + METADATA_SIZE) |= 1;

	return (char *)current_zone + METADATA_SIZE + sizeof(size_t); // skip 2 pointers and boundary + header
}

static void update_metadata(void *current_block, const size_t size)
{
	// divide block into 2 pieces if request does not occuppy all space
	if (size < *(size_t *)current_block)
	{
		void *next_footer = (char *)current_block + *(size_t *)current_block - sizeof(size_t);
		void *next_header = (char *)current_block + size;

		*(size_t *)next_header = *(size_t *)current_block - size;
		*(size_t *)next_footer = *(size_t *)next_header;
	}

	// header
	*(size_t *)current_block = size;
	*(size_t *)current_block |= 1; // set LSB

	// footer
	*(size_t *)((char *)current_block + size - sizeof(size_t)) = size;
	*(size_t *)((char *)current_block + size - sizeof(size_t)) |= 1;
}

void *malloc(size_t size)
{
	if (size == 0)
		return NULL;

	align_size(&size);

	struct s_zone *current_zone = get_zone(size);
	if (current_zone == NULL)
		return NULL;

	if (size >= SMALL)
		return create_large_allocation(size, current_zone);

	void *current_block = (char *)current_zone + METADATA_SIZE; // skip 2 pointers + boundary
	const size_t zone_size = get_zone_size(size);
	size_t taken_bytes = 0;
	size_t block_size;

	// while bit is set or block is smaller than requested size
	while (*(size_t *)current_block & 1 || *(size_t *)current_block < size)
	{
		block_size = *(size_t *)current_block;
		if (*(size_t *)current_block & 1)
			block_size -= 1;

		taken_bytes += block_size;
		if (taken_bytes >= zone_size) // the end of the zone
		{
			if (current_zone->next == NULL)
			{
				current_zone->next = allocate_zone(zone_size, current_zone);
				if (current_zone->next == NULL)
					return NULL;
			}

			current_zone = current_zone->next;
			current_block = (char *)current_zone + METADATA_SIZE; // skip 2 pointers + boundary
			taken_bytes = 0;
			continue;
		}

		current_block = (char *)current_block + block_size;
	}

	update_metadata(current_block, size);

	return (char *)current_block + sizeof(size_t); // skip header
}

__attribute__((constructor)) void init()
{
	ft_printf("Custom library loaded via LD_PRELOAD!\n");
}
