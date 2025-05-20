#include "../includes/ft_malloc_internal.h"
#include "../includes/ft_malloc.h"

static void *reallocate_memory(const size_t size, const size_t copy_bytes, void *old_alloc)
{
    void *new_alloc = malloc(size);
    if (new_alloc == NULL)
        return NULL;

    ft_memcpy(new_alloc, old_alloc, copy_bytes);
    free(old_alloc);
    return new_alloc;
}

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
        return NULL;

    size_t old_size = *(size_t *)header & ~1UL; // unset LSB
    align_size(&size);

    if (old_size < size)
        return reallocate_memory(size, old_size, ptr);

    return ptr;
}
