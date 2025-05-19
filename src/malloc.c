#include "../includes/ft_malloc_internal.h"
#include "../includes/ft_malloc.h"
#include <stdio.h>

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
			if (!current_zone) {
				write(1, "zopa!\n",7);
				return NULL;
			}
		}
		else
			current_zone = current_zone->next;
	}

	*(size_t *)((char *)current_zone + METADATA_SIZE) |= 1;

	return (char *)current_zone + METADATA_SIZE + sizeof(size_t); // skip 2 pointers and boundary + header
}

static void update_metadata(void *current_block, const size_t size, void **head)
{
	// divide block into 2 pieces if request does not occuppy all space
	if (size < *(size_t *)current_block && *(size_t *)current_block - size >= sizeof(size_t) * 4)
	{
		void *next_footer = (char *)current_block + *(size_t *)current_block - sizeof(size_t);
		void *next_header = (char *)current_block + size;

		*(size_t *)next_header = *(size_t *)current_block - size;
		*(size_t *)next_footer = *(size_t *)next_header;

		push_to_free_list(next_header, head);
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
	//char buffer[128];
	//int len = snprintf(buffer, sizeof(buffer), "Number: %ld\n", size);
	//write(1, buffer, len);
	
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
			if (!new_zone) {
				write(1, "sopa!\n",7);
				return NULL;
			}

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

		if (!*head) // in case free list is empty
		{
			void *new_zone = allocate_zone(get_zone_size(size), get_last_zone(size), NULL, NULL);
			if (!new_zone)
			{
				write(1, "popa!\n",7);
				return NULL;
			}
			push_to_free_list((char *)new_zone + METADATA_SIZE, head);
		}
		//print_free_list(*head);
		return (char *)current_free_block + sizeof(size_t);
	}

	write(1, "gopa!\n",7);
	return NULL;
}

__attribute__((constructor)) void init()
{
	write(1, "custom\n", 8);
}

 __attribute__((destructor)) void des()
{
 	write(1, "endd!\n", 7);
}
