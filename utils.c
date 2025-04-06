#include "ft_malloc.h"

void** get_zone(const int aligned_size) {
        if (aligned_size < TINY) return zones.tiny_zones;
        if (aligned_size < SMALL) return zones.small_zones;

        return NULL;
}

int get_zone_size(const int aligned_size) {
        if (aligned_size < TINY) return TINY_ZONE_SIZE;
        if (aligned_size < SMALL) return SMALL_ZONE_SIZE;

        return 0;
}
