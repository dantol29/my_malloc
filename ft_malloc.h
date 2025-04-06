#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# define ALIGNING 16
# define MAX_ZONES_COUNT 4
# define TINY 128
# define TINY_ZONE_SIZE 1024 // 1KB
# define SMALL 4096 // 4KB
# define SMALL_ZONE_SIZE 130048 // 127KB

#include <stddef.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct s_zones {
        void** tiny_zones;
        void** small_zones;
};

extern struct s_zones zones;

void* ft_malloc(size_t size);
void ft_free(void* ptr);
int init_malloc();

// utils.c
void** get_zone(const int aligned_size);
int get_zone_size(const int aligned_size);

#endif
