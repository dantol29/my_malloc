#include "../includes/ft_malloc_internal.h"
#include "../includes/ft_malloc.h"

struct s_heap heap = {0};

static void *create_large_allocation(const size_t size)
{
	struct s_zone *current_zone = get_zone(size);
	if (!current_zone)
		return NULL;

	while (*((char *)current_zone + METADATA_SIZE) & 1)
	{
		if (!current_zone->next)
		{
			current_zone = allocate_zone(size, current_zone, NULL, NULL);
			if (!current_zone)
				return NULL;
		}
		else
			current_zone = current_zone->next;
	}

	*(size_t *)((char *)current_zone + METADATA_SIZE) |= 1;

	return (char *)current_zone + METADATA_SIZE + HEADER_SIZE; // skip 2 pointers and boundary + header
}

static void update_metadata(void *current_block, const size_t size, void **head)
{
	size_t current_size = *(size_t *)current_block;

	// divide block into 2 pieces if request does not occuppy all space
	if (size < current_size && current_size - size >= MIN_SPLIT_SIZE)
	{
		const size_t remaining_size = current_size - size;
		void *next_header = (char *)current_block + size;
		void *next_footer = (char *)next_header + remaining_size - sizeof(size_t);

		*(size_t *)next_header = remaining_size;
		*(size_t *)next_footer = remaining_size;

		push_to_free_list(next_header, head);

		current_size = size;
	}

	// header size and LSB
	*(size_t *)current_block = current_size | 1;

	// footer size and LSB
	*(size_t *)((char *)current_block + current_size - sizeof(size_t)) = current_size | 1;
}

void *malloc(size_t size)
{
	if (size == 0)
		return NULL;

	align_size(&size);
	initialize_zone(size);

	if (size > SMALL)
		return create_large_allocation(size);

	void **head = get_free_list(size);
	void *current_free_block = *head;

	while (current_free_block && *(size_t *)current_free_block < size)
	{
		void *next_block = (void *)(*(uintptr_t *)((char *)current_free_block + NEXT));
		if (!next_block)
		{
			void *new_zone = allocate_zone(get_zone_size(size), get_last_zone(size), current_free_block, head);
			if (!new_zone)
				return NULL;

			push_to_free_list((char *)new_zone + METADATA_SIZE, head);
			current_free_block = *head;
			break;
		}

		current_free_block = next_block;
	}

	if (current_free_block)
	{
		remove_from_free_list(current_free_block, head, NULL);
		update_metadata(current_free_block, size, head);

		return (char *)current_free_block + HEADER_SIZE;
	}

	return NULL;
}

/*__attribute__((constructor)) void init()
{
	(void)write(1, "custom\n", 8);
}

__attribute__((destructor)) void des()
{
	(void)write(1, "endd!\n", 7);
}*/
