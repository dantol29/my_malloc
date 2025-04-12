#include "../includes/ft_malloc.h"

struct s_heap heap = {0};

static void *create_large_allocation(const int aligned_size)
{
	struct s_zone *current_zone = get_zone(aligned_size);
	if (current_zone == NULL)
		return NULL;

	while (*((int *)current_zone + 4) & 1)
	{
		if (current_zone->next == NULL)
		{
			current_zone = allocate_zone(aligned_size, current_zone);
		}
	}

	return (int *)current_zone + 5; // skip 2 pointers + size
}

static void update_metadata(void *current_chunk, int size)
{
	// divide chunk into 2 pieces if request does not occuppy all space
	if (size < *(int *)current_chunk)
	{
		void *next_footer = current_chunk + *(int *)current_chunk - 4;
		void *next_header = current_chunk + size;

		*(int *)next_header = *(int *)current_chunk - size;
		*(int *)next_footer = *(int *)next_header;
	}

	// header
	*(int *)current_chunk = size;
	*(int *)current_chunk |= 1; // set LSB

	// footer
	*(int *)(current_chunk + size - 4) = size;
	*(int *)(current_chunk + size - 4) |= 1;
}

void *malloc(size_t size)
{
	size = size + sizeof(int) * 2;										// 2 ints to store metadata(header,footer)
	const int aligned_size = (size + (ALIGNING - 1)) & ~(ALIGNING - 1); // align to the nearest multiplier of ALIGNING (16 -> 16,32,48)

	if (aligned_size >= SMALL)
		return create_large_allocation(aligned_size);

	struct s_zone *current_zone = get_zone(aligned_size);
	if (current_zone == NULL)
		return NULL;

	void *current_chunk = (int *)current_zone + 4; // skip 2 pointers
	const int zone_size = get_zone_size(aligned_size);
	int taken_bytes = 0;
	int tmp;

	// while bit is set or chunk is smaller than requested size
	while (*(int *)current_chunk & 1 || *(int *)current_chunk < aligned_size)
	{
		tmp = *(int *)current_chunk;
		if (*(int *)current_chunk & 1)
			tmp -= 1;

		taken_bytes += tmp;
		if (taken_bytes >= zone_size)
		{ // reached the end of the zone
			if (current_zone->next == NULL)
				current_zone->next = allocate_zone(zone_size, current_zone);

			current_zone = current_zone->next;
			current_chunk = (int *)current_zone + 4; // skip 2 pointers
			taken_bytes = 0;
			continue;
		}

		current_chunk = (char *)current_chunk + tmp;
	}

	update_metadata(current_chunk, aligned_size);

	return (int *)current_chunk + 1; // move pointer 4 bytes to skip header
}
