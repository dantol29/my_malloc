#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TINY 128
#define SMALL 4096 // 4KB

struct s_zone_info {
        void* start;
        unsigned int bytes_left;
};

struct s_zones {
        int is_init;
        struct s_zone_info tiny;
        struct s_zone_info small;
        struct s_zone_info large;
};

struct s_zones zones;

int init_malloc() {
        zones.tiny.start = mmap(0, TINY * 100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (zones.tiny.start == MAP_FAILED)
                return -1;

        *(int *)zones.tiny.start = 0;

        return 0;
}

void ft_free(void *ptr) {
        if (ptr == NULL)
                return;

        void* header = ptr-4;
        void* current_chunk;

        // if bit is set(chunk is allocated)
        if (*(int *)header & 1) {
                const unsigned int size = *(int *)header - 1;
                const unsigned int size_and_bit = *(int *)header;
                void* footer = header + size - 4;

                // unset LSB
                *(int *)header = size_and_bit & (size_and_bit - 1);
                *(int *)footer = size_and_bit & (size_and_bit - 1);

                // coalescing(combine free chunks)

                // if prev chunk is free
                void* prev_footer = header - 4;
                if (!(*(int *)prev_footer & 1)) {
                        printf("prev chunk is free\n");
                        void* prev_header = prev_footer - *(int *)prev_footer + 4;
                        *(int *)footer += *(int *)prev_header; // update the size
                        *(int *)prev_header += *(int *)header;
                }

                // if next chunk is free
                void* next_header = footer + 4;
                if (!(*(int *)next_header & 1)) {
                        printf("next chunk is free\n");
                        void* next_footer = next_header + *(int *)next_header - 4;
                        *(int *)(footer - *(int *)footer + 4) += *(int *)next_footer; // update the size
                        *(int *)next_footer += *(int *)footer;
                }
        }

        return;
}

void* ft_malloc(unsigned int size) {
        int total_size = size + 8; // 8 bytes of metadata(4 header, 4 footer)

        if (total_size < TINY) {
                void* current_chunk = zones.tiny.start;

                // while bit is set
                while (*(int *)current_chunk & 1) {
                        current_chunk = current_chunk + (*(int *)current_chunk - 1);
                        printf("next: %p, %d\n", current_chunk, *(int *)current_chunk);
                }

                const int aligned_size = (total_size + 7) & ~7; // align to the nearest multiplier of 8 (8,16,24,32...)
                // header
                *(int *)current_chunk = aligned_size;
                *(int *)current_chunk |= 1; // set LSB
                // footer
                *(int *)(current_chunk + aligned_size - 4) = aligned_size;
                *(int *)(current_chunk + aligned_size - 4) |= 1;
                // TODO: for the last chunk do not specify footer to know where the heap ends   
                return (int *)current_chunk + 1; // move pointer 4 bytes to skip header
        }

        return NULL;
}

int main() {
        if (init_malloc() == -1)
                return 1;

        void* ptr = ft_malloc(16);
        memset(ptr, 'A', 16);

        void* ptr2 = ft_malloc(2);
        memset(ptr2, 'B', 2);

        void* ptr3 = ft_malloc(9);
        memset(ptr3, 'C', 9);

        ft_free(ptr2);
        ft_free(ptr3);
        return 0;
}

