#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lex.c"
#include "set.c"

#define len(s) (sizeof s / sizeof s[0])

bool of(int v, Set s)
{
    for (int i = 0; i < s.len; i++) {
        if (v == s.elems[i]) {
            return true;
        }
    }

    return false;
}

bool eq(Set s, Set a)
{
    if (s.len != a.len) {
        return false;
    }

    for (int i = 0; i < a.len; i++) {
        if (of(s.elems[i], a) == false) {
            return false;
        }
    }

    return true;
}

bool subset(Set s, Set a)
{
    if (s.len >= a.len || s.len == 0) {
        return false;
    }
    
    for (int i = 0; i < a.len - 1; i++) {
        if (of(s.elems[i], a) == false) {
            return false;
        }
    }
    
    return true;
}

Set union_(Set s, Set a)
{
    Set r = s;

    for (int i = 0; i <= a.len; i++) {
        if (of(a.elems[i], s) == false) {
            add_elem(&r, s.elems[i]);
        }
    }
    
    for (int i = 0; i <= s.len; i++) {
        if (of(s.elems[i], a) == false) {
            add_elem(&r, a.elems[i]);
        }
    }

    return r;
}

Set intersect(Set s, Set a)
{
    Set r = {0};

    if (s.len >= a.len) {
        for (int i = 0; i < s.len; i++) {
            if (of(s.elems[i], a) == true) {
                add_elem(&r, s.elems[i]);
            }
        }
    } else {
        for (int i = 0; i < a.len; i++) {
            if (of(a.elems[i], s) == true) {
                add_elem(&r, a.elems[i]);
            }
        }
    }
    
    return r;
}

Set compliment(Set s, Set a)
{
    Set r = {0};

    if (s.len >= a.len) {
        for (int i = 0; i < s.len; i++) {
            if (of(s.elems[i], a) == false) {
                add_elem(&r, s.elems[i]);
            }
        }
    } else {
        for (int i = 0; i < a.len; i++) {
            if (of(a.elems[i], s) == false) {
                add_elem(&r, a.elems[i]);
            }
        }
    }

    return r;
}   

char *input()
{
    printf("-> ");
    char i[255];
    char *s = (char*) malloc(12*sizeof(char));
    fgets(i, 255, stdin);
    i[strcspn(i, "\n")] = 0;
    strcpy(s, i);
    return s;
}

int main(void)
{
    return 0;
}
