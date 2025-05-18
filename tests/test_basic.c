#include <stdlib.h>
#include "../includes/ft_malloc.h"

int main()
{
    void *m = malloc(900);
    (void)m;

    void *a = malloc(500);
    (void)a;

    void *b = malloc(2000);
    (void)b;

    void *c = malloc(2000);
    (void)c;

    void *d = malloc(20000);
    (void)d;

    // show_alloc_mem();
}
