#include <stdio.h>
#include <stdlib.h>

typedef struct Set {
    int elems[255];
    size_t len;
} Set;

void print_set(Set a)
{
    size_t l = a.len;
    int *s = a.elems;

    if (l == 0) {
        printf("{}\n");
        return;
    }
    
    printf("{");
    for (size_t i = 0; i < l - 2; i++) {
        printf("%d, ", s[i]);
    }
    printf("%d} ", s[l - 2]);
}

void add_elem(Set *s, int n)
{
    s->elems[s->len - 1] = n;
    s->len++;
}
