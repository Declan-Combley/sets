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

int main(void)
{
    Set a = {0};
    Set b = {0};

    add_elem(&a, 1);
    add_elem(&a, 2);

    add_elem(&b, 1);
    add_elem(&b, 2);
    add_elem(&b, 3);

    return 0;
}
