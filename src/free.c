#include "../includes/ft_malloc.h"

void free(void *ptr)
{
    if (ptr == NULL)
        return;

    void *header = (size_t *)ptr - 1;

    // if bit is set(chunk is allocated)
    if (*(size_t *)header & 1)
    {
        const size_t size = *(size_t *)header - 1;
        void *footer = (char *)header + size - sizeof(size_t);
        void *prev_footer = (char *)header - sizeof(size_t);
        void *next_header = (char *)footer + sizeof(size_t);

        // unset LSB to mark as free
        *(size_t *)header = size;
        *(size_t *)footer = size;

        // Coalescing with prev block
        if (*(size_t *)prev_footer != 0 && !(*(size_t *)prev_footer & 1))
        {
            void *prev_header = (char *)prev_footer - *(size_t *)prev_footer + sizeof(size_t);

            size_t new_size = *(size_t *)prev_header + *(size_t *)header;
            *(size_t *)footer = new_size;
            *(size_t *)prev_header = new_size;
        }

        // Coalescing with next block
        if (*(size_t *)next_header != 0 && !(*(size_t *)next_header & 1))
        {
            void *next_footer = (char *)next_header + *(size_t *)next_header - sizeof(size_t);

            size_t new_size = *(size_t *)(next_footer) + *(size_t *)(footer);
            *(size_t *)((char *)footer - *(size_t *)footer + sizeof(size_t)) = new_size; // update header
            *(size_t *)next_footer = new_size;
        }
    }
}
