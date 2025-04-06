#include "ft_malloc.h"

struct s_zones zones = {0};

/// find the next zone
/// request memory if does not exist
/// return null if max amount of zones already exist
void* get_next_zone(void** zones, void* current_zone, int zone_size) {
        int i = 0;
        while (zones[i] != NULL && zones[i] != current_zone && i <= MAX_ZONES_COUNT)
                ++i;

        if (i > MAX_ZONES_COUNT) {
                printf("OUT OF MEMORY!!");
                return NULL;
        }

        if (zones[i] != NULL && zones[i] == current_zone)
                ++i;

        if (zones[i] == NULL) {
                zones[i] = mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
                if (zones[i] == MAP_FAILED)
                        return NULL;

                *(int *)zones[i] = zone_size;
        }

        return zones[i];
}

int init_malloc() {
        zones.tiny_zones = mmap(0, sizeof(void*) * MAX_ZONES_COUNT + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (zones.tiny_zones == MAP_FAILED)
                return -1;

        zones.small_zones = mmap(0, sizeof(void*) * MAX_ZONES_COUNT + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (zones.small_zones == MAP_FAILED)
                return -1;

        int i = 0;
        while(i < MAX_ZONES_COUNT + 1) {
                zones.tiny_zones[i] = NULL;
                zones.small_zones[i] = NULL;
                ++i;
        }

        if (get_next_zone(zones.tiny_zones, NULL, TINY_ZONE_SIZE) == NULL)
                return -1;

        if (get_next_zone(zones.small_zones, NULL, SMALL_ZONE_SIZE) == NULL)
                return -1;

        return 0;
}

int is_first_chunk(void* chunk) {
        int i = 0;
        while (zones.tiny_zones[i]) {
                if (chunk == zones.tiny_zones[i])
                        return 1;
                ++i;
        }

        i = 0;
        while (zones.small_zones[i]) {
                if (chunk == zones.small_zones[i])
                        return 1;
                ++i;
        }

        return 0;
}

void ft_free(void *ptr) {
        if (ptr == NULL)
                return;

        void* header = ptr - 4;
        void* current_chunk;

        // if bit is set(chunk is allocated)
        if (*(int *)header & 1) {
                const unsigned int size_and_bit = *(int *)header;
                void* footer = header + size_and_bit - 1 - 4;
                void* prev_footer = header - 4;
                void* next_header = footer + 4;

                // unset LSB
                *(int *)header = size_and_bit & (size_and_bit - 1);
                *(int *)footer = size_and_bit & (size_and_bit - 1);

                // coalescing(combine free chunks)

                // if chunk is not 1st one and prev chunk is free
                if (!is_first_chunk(header) && !(*(int *)prev_footer & 1)) {
                        //printf("prev chunk is free\n");
                        void* prev_header = prev_footer - *(int *)prev_footer + 4;
                        *(int *)footer += *(int *)prev_header; // update the size
                        *(int *)prev_header += *(int *)header;
                }

                // if next chunk is free
                if (!(*(int *)next_header & 1)) {
                        //printf("next chunk is free\n");
                        void* next_footer = next_header + *(int *)next_header - 4;
                        // dynamically get header because it might combined with prev chunk
                        *(int *)(footer - *(int *)footer + 4) += *(int *)next_footer; // update header
                        *(int *)next_footer += *(int *)footer;
                }
        }

        return;
}

/// update header and footer
void update_metadata(void* current_chunk, int size) {
        // divide chunk into 2 pieces if request does not occuppy all space
        if (size < *(int *)current_chunk) {
                void* next_footer = current_chunk + *(int *)current_chunk - 4;
                void* next_header = current_chunk + size;
                *(int *)next_header = *(int *)current_chunk - size;
                *(int *)next_footer = *(int *)next_header;
        }

        // header
        *(int *)current_chunk = size;
        *(int *)current_chunk |= 1; // set LSB

        // footer
        *(int *)(current_chunk + size - 4) = size;
        *(int *)(current_chunk + size - 4) |= 1;
}

void* ft_malloc(unsigned int size) {
        int total_size = size + 8; // 8 bytes of metadata(4 header, 4 footer)
        const int aligned_size = (total_size + (ALIGNING - 1)) & ~(ALIGNING - 1); // align to the nearest multiplier of ALIGNING (16 -> 16,32,48)

        void* current_zone = *get_zone(aligned_size);
        void* current_chunk = current_zone;
        int zone_size = get_zone_size(aligned_size);

        int taken_bytes = 0;
        int tmp;

        // while bit is set or chunk is smaller than requested size
        while (*(int *)current_chunk & 1 || *(int *)current_chunk < aligned_size) {
                tmp = *(int *)current_chunk;
                if (*(int *)current_chunk & 1)
                        tmp -= 1;

                taken_bytes += tmp;
                if (taken_bytes >= zone_size) { // reached the end of the zone
                        printf("SWITCHING ZONE!\n");
                        current_zone = get_next_zone(get_zone(aligned_size), current_zone, zone_size);
                        if (current_zone == NULL)
                                return NULL;

                        current_chunk = current_zone;
                        taken_bytes = 0;
                        continue ;
                }

                current_chunk += tmp;
        }

        update_metadata(current_chunk, aligned_size);

        return (int *)current_chunk + 1; // move pointer 4 bytes to skip header
}
