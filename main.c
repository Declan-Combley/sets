#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lex.c"

#define len(s) (sizeof s / sizeof s[0])

void print(int *s, size_t l)
{
    if (l == 0) {
        printf("{}\n");
        return;
    }
    
    printf("{");
    for (size_t i = 0; i < l - 1; i++) {
        printf("%d, ", s[i]);
    }
    printf("%d}\n", s[l - 1]);
}

bool is_in(int v, int *s, size_t sz)
{
    for (size_t i = 0; i < sz; i++) {
        if (v == s[i]) {
            return true;
        }
    }

    return false;
}

void compliment(int *s, size_t sz_s, int *a, size_t sz_a)
{
    int elems[sz_s + sz_a];
    size_t count = 0;

    if (sz_s >= sz_a) {
        for (size_t i = 0; i < sz_s; i++) {
            if (is_in(s[i], a, sz_a) == false) {
                elems[count] = s[i];
                count++;
            }
        }
    } else {
        for (size_t i = 0; i < sz_a; i++) {
            if (is_in(a[i], s, sz_s) == false) {
                elems[count] = a[i];
                count++;
            }
        }        
    }
    
    print(elems, count);    
}

void intersect(int *s, size_t sz_s, int *a, size_t sz_a)
{
    int elems[sz_s + sz_a];
    size_t count = 0;

    if (sz_s >= sz_a) {
        for (size_t i = 0; i < sz_s; i++) {
            if (is_in(s[i], a, sz_a) == true) {
                elems[count] = s[i];
                count++;
            }
        }
    } else {
        for (size_t i = 0; i < sz_a; i++) {
            if (is_in(a[i], s, sz_s) == true) {
                elems[count] = a[i];
                count++;
            }
        }        
    }
    
    print(elems, count);    
}

void unions(int *s, size_t sz_s, int *a, size_t sz_a)
{
    int elems[sz_s + sz_a];
    size_t count = 0;

    for (size_t i = 0; i < sz_s; i++) {
        if (is_in(s[i], elems, count) == false) {
            elems[count] = s[i];
            count++;
        }
    } 
    for (size_t i = 0; i < sz_a; i++) {
        if (is_in(a[i], elems, count) == false) {
            elems[count] = a[i];
            count++;
        }
    }
    
    print(elems, count);
}

bool subset(int *s, size_t sz_s, int *a, size_t sz_a)
{
    if (sz_a > sz_s) {
        printf("false\n");
        return false;
    }

    
    for (size_t i = 0; i < sz_a; i++) {
        if (is_in(s[i], a, sz_a) == false) {
            printf("false\n");
            return false;
        }
    }
    
    printf("true\n");
    return true;
}

int main(void)
{
    int a[3] = {1, 2, 3};
    int b[4] = {3, 4, 5, 6};
    unions(a, len(a), b, len(b));
    intersect(a, len(a), b, len(b));
    compliment(a, len(a), b, len(b));
    subset(a, len(a), b, len(b));
    
    return 0;
}
