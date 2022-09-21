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
    Hash hash[26 * 2];
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
    printf("     %c -> ", h.name);
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

void start() {
    while (1) {
        printf("-> ");
        char in[255];
        fgets(in, 255, stdin);
        in[strcspn(in, "\n")] = 0;

        Token *t = tokenize(in);
        bool c = true;

        if (t[0].kind == Character && t[1].kind != End) {
            if (t[1].kind != Eq) {
                fprintf(stderr, "    %c must be initialized\n", t[0].value);
                fprintf(stderr, "       | try > %c = {...}\n", t[0].value);
                continue;
            }
            if (t[2].kind != OpenBracket || t[2].kind == End) {
                fprintf(stderr, "    %c expected an opening bracket\n", t[0].value);
                fprintf(stderr, "       | try > %c = {...}\n", t[0].value);
                continue;
            }
            if (t[3].kind == End) {
                fprintf(stderr, "    %c must be intiialized\n", t[0].value);
                fprintf(stderr, "       | try > %c = {...}\n", t[0].value);
                continue;
            }
            if (t[3].kind == CloseBracket) {
                fprintf(stderr, "    Set %c must have at least one element\n", t[0].value);
                continue;
            }
            if (t[3].kind != Num) {
                if (t[3].kind == Character) {
                    fprintf(stderr, "    Set %c must contain only numbers: %c is invalid\n", t[0].value, t[3].value);
                } else {
                    fprintf(stderr, "    Set %c must contain only numbers: %s is invalid\n", t[0].value, t[3].string);
                }
                
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
                    if (t[i].kind == Character) {
                        fprintf(stderr, "    Set %c must contain only numbers: %c is invalid\n", t[0].value, t[i].value);
                    } else {
                        fprintf(stderr, "    Set %c must contain only numbers: %s is invalid\n", t[0].value, t[i].string);
                    }
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
                    if (t[i].kind == String || t[i].kind == Character) {
                        if (t[i].kind == Character) {
                            fprintf(stderr, "    Set %c must contain only numbers, element %c is invalid\n", t[0].value, t[i].value);
                        } else {
                            fprintf(stderr, "    Set %c must contain only numbers, element %s is invalid\n", t[0].value, t[i].string);
                        }
                        c = false;
                        break;
                    } else if (t[i + 1].kind == String || t[i + 1].kind == Character) {
                        if (t[i].kind == Character) {
                            fprintf(stderr, "    Set %c must contain only numbers, element %c is invalid\n", t[0].value, t[i + 1].value);
                        } else {
                            fprintf(stderr, "    Set %c must contain only numbers, element %s is invalid\n", t[0].value, t[i + 1].string);
                        }
                        c = false;
                        break;
                    }
                    
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

            if (c) {
                Hash h;
                Set tmp;
                tmp.len = 0;
                h.name = t[0].value;
                
                i = 3;
                while (t[i].kind != CloseBracket && t[i].kind != End) {
                    if (t[i].kind != Comma) {
                        add_elem(&tmp, t[i].num);
                    }
                    i++;
                }
                h.name = t[0].value;
                h.err = false;
                h.set = tmp;
                insert_hash(&sets, h);
                print_hash(get_hash_from_table(t[0].value, sets));
            }
        }
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
