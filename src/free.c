#include "../includes/ft_malloc_internal.h"
#include <stdio.h> // perror

static void free_large_allocation(void *ptr, size_t size)
{
    const size_t additional_metadata = sizeof(void *) * 2 + sizeof(size_t) * 2; // 2 pointers and 2 boundaries
    const size_t total_size = size + additional_metadata;                       // TODO: double check this, might be more

    struct s_zone *beggining = (struct s_zone *)((char *)ptr - additional_metadata);
    if (beggining == NULL)
        return;

    if (beggining->prev == NULL) // first zone
    {
        if (beggining->next)
        {
            heap.large_zone = beggining->next;
            beggining->next->prev = NULL;
        }
        else
            heap.large_zone = NULL;
    }
    else
    {
        if (beggining->next)
        {
            beggining->prev->next = beggining->next;
            beggining->next->prev = beggining->prev;
        }
        else // last zone
        {
            if (beggining->prev->next)
                beggining->prev->next = NULL;
        }
    }

    if (munmap((void *)beggining, total_size) == -1)
        perror("Failed to deallocate with munmap");
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;

    void *header = (size_t *)ptr - 1;

    // if bit is set(chunk is allocated)
    if (*(size_t *)header & 1)
    {
        const size_t size = *(size_t *)header - 1;
        if (size > SMALL)
            return free_large_allocation(ptr, size);

        void *footer = (char *)header + size - sizeof(size_t);
        void *prev_footer = (char *)header - sizeof(size_t);
        void *prev_header = NULL;
        void *next_header = (char *)footer + sizeof(size_t);
        void **free_list_head = get_free_list(size);

        // unset LSB to mark as free
        *(size_t *)header = size;
        *(size_t *)footer = size;

        ft_printf("freeing!\n");

        // Coalescing with prev block
        if (*(size_t *)prev_footer != 0 && !(*(size_t *)prev_footer & 1))
        {
            prev_header = (char *)prev_footer - *(size_t *)prev_footer + sizeof(size_t);

            size_t new_size = *(size_t *)prev_header + *(size_t *)header;
            *(size_t *)footer = new_size;
            *(size_t *)prev_header = new_size;

            remove_from_free_list(prev_header, free_list_head);
        }

        // Coalescing with next block
        if (*(size_t *)next_header != 0 && !(*(size_t *)next_header & 1))
        {
            void *next_footer = (char *)next_header + *(size_t *)next_header - sizeof(size_t);

            size_t new_size = *(size_t *)(next_footer) + *(size_t *)(footer);
            *(size_t *)((char *)footer - *(size_t *)footer + sizeof(size_t)) = new_size; // update header
            *(size_t *)next_footer = new_size;

            remove_from_free_list(next_header, free_list_head);
        }

        if (prev_header)
            push_to_free_list(prev_header, free_list_head);
        else
            push_to_free_list(header, free_list_head);
    }
}
