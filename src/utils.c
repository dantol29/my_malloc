#include "../includes/ft_malloc_internal.h"

void *ft_memcpy(void *dest, const void *src, size_t n)
{
        size_t i;
        unsigned char *d;
        const unsigned char *s;

        d = (unsigned char *)dest;
        s = (unsigned char *)src;
        i = 0;
        while (i < n)
        {
                d[i] = s[i];
                ++i;
        }
        return dest;
}

inline void align_size(size_t *size)
{
        if (!size)
                return;

        *size = *size + sizeof(size_t) * 2;                 // 2 fields to store header and footer
        *size = (*size + (ALIGNING - 1)) & ~(ALIGNING - 1); // align to the nearest multiplier of ALIGNING (16 -> 16,32,48)
}
