#include "../includes/ft_malloc.h"

void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }

    void *header = (size_t *)ptr - 1;
    if (header == NULL || !(*(size_t *)header & 1))
        return malloc(size);

    size_t old_size = *(size_t *)header - 1;
    const size_t size_with_metadata = size + sizeof(size_t) * 2;
    const size_t aligned_size = (size_with_metadata + (ALIGNING - 1)) & ~(ALIGNING - 1);

    if (old_size < aligned_size)
    {
        void *new_alloc = malloc(size);
        if (new_alloc == NULL)
            return NULL;

        ft_memcpy(new_alloc, ptr, old_size);
        free(ptr);
        return new_alloc;
    }
    else if (old_size > aligned_size)
    {
        void *new_alloc = malloc(size);
        if (new_alloc == NULL)
            return NULL;

        ft_memcpy(new_alloc, ptr, size);
        free(ptr);
        return new_alloc;
    }
    else
    {
        return ptr;
    }
}
