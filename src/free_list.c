#include "../includes/ft_malloc_internal.h"
#include "../includes/ft_malloc.h"

void push_to_free_list(void *free_block, void **head)
{
    if (!head)
        return;

    *(uintptr_t *)((char *)free_block + NEXT) = (uintptr_t)*head;
    *(uintptr_t *)((char *)free_block + PREV) = 0;

    if (*head) {
	 //print_free_list(*head);
	 *(uintptr_t *)((char *)*head + PREV) = (uintptr_t)free_block;
    }
    *head = free_block;
}

void remove_from_free_list(void *free_block, void **head, void* header)
{
    (void)header;
    if (!head || !free_block)
        return;

    void *next = (void *)(*(uintptr_t *)((char *)free_block + NEXT));
    void *prev = (void *)(*(uintptr_t *)((char *)free_block + PREV));

    if (prev)
        *(uintptr_t *)((char *)prev + NEXT) = (uintptr_t)next;
    else
        *head = next;

    if (next) {
	//print_free_list(heap.tiny_free_list_head);
	//print_free_list(heap.small_free_list_head);
        *(uintptr_t *)((char *)next + PREV) = (uintptr_t)prev;
	}
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

static void size_t_to_str(size_t num, char *buf, size_t bufsize) {
    // buf must be big enough (at least 21 bytes for 64-bit)
    if (bufsize == 0) return;

    buf[bufsize - 1] = '\0';

    size_t i = bufsize - 2;
    if (num == 0) {
        buf[i] = '0';
        i--;
    } else {
        while (num > 0 && i < bufsize) {
            buf[i] = '0' + (num % 10);
            num /= 10;
            i--;
        }
    }

    // Shift the string to the front of the buffer
    size_t start = i + 1;
    size_t j = 0;
    while (buf[start] != '\0' && j < bufsize) {
        buf[j++] = buf[start++];
    }
    buf[j] = '\0';
}

void print_free_list(void *head) {
    void *current_free_block = head;
    const char start_msg[] = "START OF FREE LIST!\n";
    const char end_msg[] = "END OF FREE LIST!\n";
    const char block_prefix[] = "block size: ";
    const char newline = '\n';

    write(STDOUT_FILENO, start_msg, sizeof(start_msg) - 1);

    char numbuf[32];  // buffer to hold size_t as string

    while (current_free_block) {
        size_t block_size = *(size_t *)current_free_block;

        // Write prefix
        write(STDOUT_FILENO, block_prefix, sizeof(block_prefix) - 1);

        // Convert block size to string
        size_t_to_str(block_size, numbuf, sizeof(numbuf));

        // Write block size string
        size_t len = 0;
        while (numbuf[len] != '\0') len++;
        write(STDOUT_FILENO, numbuf, len);

        // Write newline
        write(STDOUT_FILENO, &newline, 1);

        // Move to next free block pointer
        current_free_block = (void *)(*(uintptr_t *)((char *)current_free_block + NEXT));
    }

    write(STDOUT_FILENO, end_msg, sizeof(end_msg) - 1);
}

/*#include <stdio.h>
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
}*/
