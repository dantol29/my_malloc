#include "ft_malloc.h"

void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);

    void *header = (int *)ptr - 1;
    if (header == NULL || !(*(int *)header & 1))
        return malloc(size);

    int old_size = *(int *)header - 1;
    const int size_with_metadata = size + sizeof(int) * 2;
    const int aligned_size = (size_with_metadata + (ALIGNING - 1)) & ~(ALIGNING - 1);

    if (old_size < aligned_size)
    {
        void *new_alloc = malloc(size);
        if (new_alloc == NULL)
            return NULL;

        memcpy(new_alloc, ptr, old_size);
        free(ptr);
        return new_alloc;
    }
    else if (old_size > aligned_size)
    {
        void *new_alloc = malloc(size);
        if (new_alloc == NULL)
            return NULL;

        memcpy(new_alloc, ptr, size);
        free(ptr);
        return new_alloc;
    }
    else
    {
        return ptr;
    }
}
