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

Set unions(Set s, Set a)
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

int main(void)
{
    Set a = {0};
    Set b = {0};

    return 0;
}
