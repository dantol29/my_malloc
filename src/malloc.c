#include "../includes/ft_malloc.h"

struct s_heap heap = {0};

static void *create_large_allocation(const size_t aligned_size)
{
	struct s_zone *current_zone = get_zone(aligned_size);
	if (current_zone == NULL)
		return NULL;

	while (*((char *)current_zone + METADATA_SIZE) & 1)
	{
		if (current_zone->next == NULL)
		{
			current_zone = allocate_zone(aligned_size, current_zone);
		}
		else
		{
			current_zone = current_zone->next;
		}
	}

	return (char *)current_zone + METADATA_SIZE + sizeof(size_t); // skip 2 pointers and boundary + size
}

static void update_metadata(void *current_block, size_t size)
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
	size = size + sizeof(size_t) * 2;									   // 2 fields to store header and footer
	const size_t aligned_size = (size + (ALIGNING - 1)) & ~(ALIGNING - 1); // align to the nearest multiplier of ALIGNING (16 -> 16,32,48)
	const size_t zone_size = get_zone_size(aligned_size);

	if (aligned_size >= SMALL)
		return create_large_allocation(aligned_size);

	struct s_zone *current_zone = get_zone(aligned_size);
	if (current_zone == NULL)
		return NULL;

	void *current_block = (char *)current_zone + METADATA_SIZE; // skip 2 pointers + boundary
	size_t taken_bytes = 0;
	size_t tmp;

	// while bit is set or block is smaller than requested size
	while (*(size_t *)current_block & 1 || *(size_t *)current_block < aligned_size)
	{
		tmp = *(size_t *)current_block;
		if (*(size_t *)current_block & 1)
			tmp -= 1;

		taken_bytes += tmp;
		if (taken_bytes >= zone_size)
		{ // the end of the zone
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

		current_block = (char *)current_block + tmp;
	}

	update_metadata(current_block, aligned_size);

	return (char *)current_block + sizeof(size_t); // skip header
}
