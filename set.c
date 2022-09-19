#include <stdio.h>
#include <stdlib.h>

typedef struct Set {
    int elems[255];
    int len;
} Set;

void print_set(Set a)
{
    int l = a.len;
    int *s = a.elems;

    if (l == 0) {
        printf("{}\n");
        return;
    }
    
    printf("{");
    for (int i = 0; i < l - 1; i++) {
        printf("%d, ", s[i]);
    }
    printf("%d}\n", s[l - 1]);
}

void add_elem(Set *s, int n)
{
    if (n == 0) {
        return;
    }
    
    for (int i = 0; i < s->len; i++) {
        if (n == s->elems[i]) {
            return;
        }
    }
    s->elems[s->len] = n;
    s->len++;
}

void print_bool(bool b)
{
    fputs((b) ? "true": "false", stdout);
}    
