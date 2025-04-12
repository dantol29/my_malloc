#include "../includes/ft_malloc.h"

void free(void *ptr)
{
    if (ptr == NULL)
        return;

    void *header = ptr - 4;

    // if bit is set(chunk is allocated)
    if (*(int *)header & 1)
    {
        int new_size = 0;
        const int size = *(int *)header - 1;
        void *footer = header + size - 4;
        void *prev_footer = header - 4;
        void *next_header = footer + 4;

        // unset LSB to mark as free
        *(int *)header = size;
        *(int *)footer = size;

        // Coalescing with prev block
        if (prev_footer == NULL && !(*(int *)prev_footer & 1))
        {
            void *prev_header = prev_footer - *(int *)prev_footer + 4;

            new_size = *(int *)prev_header + *(int *)header;
            *(int *)footer = new_size;
            *(int *)prev_header = new_size;
        }

        // Coalescing with next block
        if (next_header != NULL && !(*(int *)next_header & 1))
        {
            void *next_footer = next_header + *(int *)next_header - 4;

            new_size = *(int *)(next_footer) + *(int *)(footer);
            *(int *)(footer - *(int *)footer + 4) = new_size; // update header
            *(int *)next_footer = new_size;
        }
    }
}
