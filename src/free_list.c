#include "../includes/ft_malloc_internal.h"

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

void remove_from_free_list(void *free_block, void **head)
{
    if (!head)
        return;

    void *next = (void *)(*(uintptr_t *)((char *)free_block + NEXT));
    void *prev = (void *)(*(uintptr_t *)((char *)free_block + PREV));

    if (prev)
        *(uintptr_t *)((char *)prev + NEXT) = (uintptr_t)next;
    else
        *head = next;

    if (next)
        *(uintptr_t *)((char *)next + PREV) = (uintptr_t)prev;
}

inline void **get_free_list(const size_t size)
{
    if (size <= TINY)
        return &heap.tiny_free_list_head;
    if (size <= SMALL)
        return &heap.small_free_list_head;
    else
        return NULL;
}

#include <stdio.h>
void print_free_list(void *head)
{
    void *current_free_block = head;

    printf("START OF FREE LIST!\n");

    while (current_free_block)
    {
        printf("block size: %ld\n", *(size_t *)current_free_block);
        current_free_block = (void *)(*(uintptr_t *)((char *)current_free_block + NEXT));
    }

    printf("END OF FREE LIST!\n");
}
