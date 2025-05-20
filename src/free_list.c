#include "../includes/ft_malloc_internal.h"
#include "../includes/ft_malloc.h"

void push_to_free_list(void *free_block, void **head)
{
    if (!head)
        return;

    *(uintptr_t *)((char *)free_block + NEXT) = (uintptr_t)*head;
    *(uintptr_t *)((char *)free_block + PREV) = 0;

    if (*head)
        *(uintptr_t *)((char *)*head + PREV) = (uintptr_t)free_block;
    *head = free_block;
}

void remove_from_free_list(void *free_block, void **head, void *header)
{
    (void)header;
    if (!head || !free_block)
        return;

    void *next = (void *)(*(uintptr_t *)((char *)free_block + NEXT));
    void *prev = (void *)(*(uintptr_t *)((char *)free_block + PREV));

    if (prev && (uintptr_t)prev % ALIGNING == 0)
        *(uintptr_t *)((char *)prev + NEXT) = (uintptr_t)next;
    else
        *head = next;

    if (next && ((char *)next + PREV) && (uintptr_t)next % ALIGNING == 0)
        *(uintptr_t *)((char *)next + PREV) = (uintptr_t)prev;

    *(uintptr_t *)((char *)free_block + NEXT) = 0;
    *(uintptr_t *)((char *)free_block + PREV) = 0;
}

inline void **get_free_list(const size_t size)
{
    void *new_zone = NULL;

    if (size <= TINY)
    {
        if (!heap.tiny_free_list_head)
        {
            new_zone = allocate_zone(TINY_ZONE_SIZE, NULL, NULL);
            push_to_free_list((char *)new_zone + METADATA_SIZE, &heap.tiny_free_list_head);
        }
        return &heap.tiny_free_list_head;
    }
    if (size <= SMALL)
    {
        if (!heap.small_free_list_head)
        {
            new_zone = allocate_zone(SMALL_ZONE_SIZE, NULL, NULL);
            push_to_free_list((char *)new_zone + METADATA_SIZE, &heap.small_free_list_head);
        }
        return &heap.small_free_list_head;
    }
    else
        return NULL;
}
