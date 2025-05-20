#include "../includes/ft_malloc_internal.h"
#include "../includes/ft_malloc.h"

static void free_large_allocation(void *ptr, size_t size)
{
    const size_t additional_metadata = sizeof(void *) * 2 + sizeof(size_t) * 2; // 2 pointers and 2 boundaries
    const size_t total_size = size + additional_metadata;

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
        return;
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;

    void *header = (char *)ptr - HEADER_SIZE;

    // if LSB is not set(block is not allocated)
    if (!(*(size_t *)header & 1))
        return;

    const size_t size = *(size_t *)header - 1;
    if (size > SMALL)
        return free_large_allocation(ptr, size);

    void *footer = (char *)header + size - sizeof(size_t);
    void *prev_footer = (char *)header - sizeof(size_t);
    void *next_header = (char *)footer + sizeof(size_t);
    void **free_list_head = get_free_list(size);

    // Mark as free by unsettting LSB
    *(size_t *)header = size;
    *(size_t *)footer = size;

    void *final_header = header;
    size_t final_size = size;

    // Coalescing with prev block
    if (*(size_t *)prev_footer != 0 && !(*(size_t *)prev_footer & 1) && *(size_t *)prev_footer % ALIGNING == 0)
    {
        const size_t prev_size = *(size_t *)prev_footer;
        void *prev_header = (char *)prev_footer - prev_size + sizeof(size_t);

        if (prev_header && *(size_t *)prev_header == prev_size)
        {
            remove_from_free_list(prev_header, free_list_head, header);
            final_header = prev_header;
            final_size = size + prev_size;
        }
    }

    // Coalescing with next block
    if (*(size_t *)next_header != 0 && !(*(size_t *)next_header & 1) && *(size_t *)next_header % ALIGNING == 0)
    {
        const size_t next_size = *(size_t *)next_header;
        void *next_footer = (char *)next_header + next_size - sizeof(size_t);

        if (next_footer && *(size_t *)next_footer == next_size)
        {
            remove_from_free_list(next_header, free_list_head, header);
            final_size += next_size;
        }
    }

    *(size_t *)final_header = final_size;
    void *final_footer = (char *)final_header + final_size - sizeof(size_t);
    *(size_t *)final_footer = final_size;

    push_to_free_list(final_header, free_list_head);
}
