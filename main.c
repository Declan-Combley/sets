#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lex.c"
#include "set.c"

typedef struct Hash {
    char name;
    bool err;
    Set set;
} Hash;

typedef struct Table {
    Hash hash[26];
    size_t index;
} Table;

void insert_hash(Table *t, Hash h)
{
    t->hash[t->index] = h;
    t->index++;
}

Hash get_hash_from_table(char h, Table t)
{
    for (size_t i = 0; i < t.index; i++) {
        if (t.hash[i].name == h) {
            return t.hash[i];
        }
    }

    t.hash[0].err = true;
    return t.hash[0];
}

void print_hash(Hash h)
{
    printf("%c -> ", h.name);
    print_set(h.set);
}

void print_table(Table t)
{
    for (size_t i = 0; i < t.index; i++) {
        printf("%c -> ", t.hash[i].name);
        print_set(t.hash[i].set);
    }
}

Table sets = {0};

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
    char *s = (char *) malloc(255 * sizeof(char));
    fgets(i, 255, stdin);
    i[strcspn(i, "\n")] = 0;
    strcpy(s, i);
    return s;
}

void start() {
    while (1) {
        char *in = input();
        Token *t = tokenize(in);
        bool c = true;

        if (t[0].kind == Character) {
            if (t[1].kind != Eq) {
                fprintf(stderr, "    %c must be initialized\n", t[0].value);
                fprintf(stderr, "       | try > '%c' = {...}\n", t[0].value);
                continue;
            }
            if (t[2].kind != OpenBracket || t[2].kind == End) {
                fprintf(stderr, "    %c expected an opening bracket\n", t[0].value);
                fprintf(stderr, "       | try > '%c' = {...}\n", t[0].value);
                continue;
            }
            if (t[3].kind == End) {
                fprintf(stderr, "    %c must be intiialized\n", t[0].value);
                fprintf(stderr, "       | try > '%c' = {...}\n", t[0].value);
                continue;
            }
            if (t[3].kind == CloseBracket) {
                fprintf(stderr, "    Set %c must have at least one element\n", t[0].value);
                continue;
            }
            if (t[3].kind != Num) {
                fprintf(stderr, "    Set %c must contain only numbers: Type %s is invalid\n", t[0].value, tokens[t[3].kind]);
                continue;
            }

            int i = 4;
            while (t[i].kind != End) {
                if (t[i].kind == CloseBracket && t[i + 1].kind != End) {
                    fprintf(stderr, "   Set is already complete\n");
                    fprintf(stderr, "       | try > %c = {%d, ", t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp + 1].kind != End && t[tmp + 1].kind != CloseBracket) {
                        if (t[tmp].kind != Comma) {
                            printf("%d, ", t[tmp].num);
                        }
                        tmp++;
                    }
                    printf("%d}\n", t[tmp].num);
                    c = false;
                    break;
                }
                if (t[i].kind != Num && t[i].kind != Comma && t[i].kind != CloseBracket) {
                    fprintf(stderr, "    Set %c must contain only numbers: Type %s is invalid\n", t[0].value, tokens[t[3].kind]);
                    c = false;
                    break;
                }
                if (i % 2 == 0 && t[i].kind != Comma && t[i].kind != CloseBracket) {
                    fprintf(stderr, "    Each element of a set must be seperated by a comma\n");
                    fprintf(stderr, "       | try > %c = {%d, ", t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp].kind != End && t[tmp].kind != CloseBracket) {
                        if (tmp % 2 != 0) {
                            printf("%d, ", t[tmp].num);
                        } 
                        tmp++;
                    }
                    printf("%d}\n", t[tmp - 1].num);

                    c = false;
                    break;
                }
                if (i % 2 == 0 && t[i].kind != CloseBracket && t[i + 1].kind != Num) {
                    fprintf(stderr, "    Expected another element\n");
                    fprintf(stderr, "       | try > %c = {%d, ", t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp].kind != End && t[tmp].kind != CloseBracket) {
                        if (tmp % 2 != 0) {
                            printf("%d, ", t[tmp].num);
                        } 
                        tmp++;
                    }
                    printf("x}\n");
                    c = false;
                    break;
                }
                if (t[i + 1].kind == End && t[i].kind != CloseBracket) {
                    fprintf(stderr, "    Expected a closing bracket\n");
                    fprintf(stderr, "       | try > %c = {%d, ", t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp + 1].kind != End) {
                        if (tmp % 2 != 0) {
                            printf("%d, ", t[tmp].num);
                        } 
                        tmp++;
                    }
                    printf("%d}\n", t[tmp].num);
                    c = false;
                    break;
                }
                i++;
            }
        }

        if (!c) {
            continue;
        }
        
        print_tokens(t);
        free(in);
    }
}

int main(void)
{
    sets.hash[0].err = 0;
    sets.hash[0].name = 'E';
    sets.index++;

    start();

    return 0;
}
