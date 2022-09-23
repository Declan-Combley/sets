#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lex.c"
#include "set.c"

const char *buff = "   ";

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

bool is_in_table(char h, Table t)
{
    for (size_t i = 0; i < t.index; i++) {
        if (t.hash[i].name == h) {
            return true;
        }
    }

    return false;
}

void print_hash(Hash h)
{
    fputs(buff, stdout);
    printf("%c -> ", h.name);
    print_set(h.set);
}

void print_table(Table t)
{
    for (size_t i = 0; i < t.index; i++) {
        fputs(buff, stdout);
        printf("%c -> ", t.hash[i].name);
        print_set(t.hash[i].set);
    }
}

void print_bool(bool b)
{
    fputs(buff, stdout);
    fputs((b) ? "true": "false", stdout);
    fputs("\n", stdout);
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
        bool new_set = false;

        if (t[0].kind == Character && t[1].kind == Eq) {
            // TODO: Fix it allowing 'x = {1'
            if (t[2].kind != OpenBracket || t[3].kind == End) {
                fprintf(stderr, "%s%c must be initialized\n", buff, t[0].value);
                fprintf(stderr, "%s   | try -> %c = {...}\n", buff, t[0].value);
                continue;
            } else if (t[2].kind != OpenBracket || t[2].kind == End) {
                fprintf(stderr, "%s%c Expected an opening bracket\n", buff, t[0].value);
                fprintf(stderr, "%s   | try -> %c = {...}\n        ", buff, t[0].value);
                continue;
            } else if (t[3].kind == CloseBracket) {
                fprintf(stderr, "%sSet %c must have at least one element\n", buff, t[0].value);
                continue;
            } else if (t[3].kind != Num) {
                if (t[3].kind == Character) {
                    fprintf(stderr, "%sSet %c must contain only numbers, %c is invalid\n", buff, t[0].value, t[3].value);
                } else {
                    fprintf(stderr, "%sSet %c must contain only numbers, %s is invalid\n", buff, t[0].value, t[3].string);
                }
                continue;
            }

            new_set = true;
            int i = 4;
            while (t[i].kind != End && new_set) {
                if (t[i].kind == CloseBracket && t[i + 1].kind != End) {
                    fprintf(stderr, "%sSet is already complete\n", buff);
                    fprintf(stderr, "%s   | try -> %c = {%d, ", buff, t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp + 1].kind != End && t[tmp + 1].kind != CloseBracket) {
                        if (t[tmp].kind != Comma) {
                            printf(" %d, ", t[tmp].num);
                        }
                        tmp++;
                    }
                    printf(" %d}\n", t[tmp].num);
                    new_set = false;
                    break;
                } else if (t[i].kind != Num && t[i].kind != Comma && t[i].kind != CloseBracket) {
                    if (t[i].kind == Character) {
                        fprintf(stderr, "%sSet %c must contain only numbers,  %c is invalid\n", buff, t[0].value, t[i].value);
                    } else {
                        fprintf(stderr, "%sSet %c must contain only numbers,  %s is invalid\n", buff, t[0].value, t[i].string);
                    }
                    new_set = false;
                    break;
                } else if (i % 2 == 0 && t[i].kind != Comma && t[i].kind != CloseBracket) {
                    fprintf(stderr, "%sEach element of a set must be seperated by a comma\n", buff);
                    fprintf(stderr, "%s   | try -> %c = {%d, ", buff, t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp].kind != End && t[tmp].kind != CloseBracket) {
                        if (tmp % 2 != 0) {
                            printf(" %d, ", t[tmp].num);
                        } 
                        tmp++;
                    }
                    printf(" %d}\n", t[tmp - 1].num);
                    new_set = false;
                    break;
                } else if (i % 2 == 0 && t[i].kind != CloseBracket && t[i + 1].kind != Num) {
                    if (t[i].kind == String || t[i].kind == Character) {
                        if (t[i].kind == Character) {
                            fprintf(stderr, "%sSet %c must contain only numbers, element %c is invalid\n", buff, t[0].value, t[i].value);
                        } else {
                            fprintf(stderr, "%sSet %c must contain only numbers, element %s is invalid\n", buff, t[0].value, t[i].string);
                        }
                        new_set = false;
                        break;
                    } else if (t[i + 1].kind == String || t[i + 1].kind == Character) {
                        if (t[i].kind == Character) {
                            fprintf(stderr, "%sSet %c must contain only numbers, element %c is invalid\n", buff , t[0].value, t[i + 1].value);
                        } else {
                            fprintf(stderr, "%sSet %c must contain only numbers, element %s is invalid\n", buff,  t[0].value, t[i + 1].string);
                        }
                        new_set = false;
                        break;
                    }
                    
                    fprintf(stderr, "%sExpected another element\n", buff);
                    fprintf(stderr, "%s   | try -> %c = {%d, ", buff , t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp].kind != End && t[tmp].kind != CloseBracket) {
                        if (tmp % 2 != 0) {
                            printf(" %d, ", t[tmp].num);
                        } 
                        tmp++;
                    }
                    printf(" x}\n");
                    new_set = false;
                    break;
                } else if (t[i + 1].kind == End && t[i].kind != CloseBracket) {
                    fprintf(stderr, "%sExpected a closing bracket\n", buff);
                    fprintf(stderr, "%s   | try -> %c = {%d, ", buff, t[0].value, t[3].num);
                    int tmp = 4;
                    while (t[tmp + 1].kind != End) {
                        if (tmp % 2 != 0) {
                            printf(" %d, ", t[tmp].num);
                        } 
                        tmp++;
                    }
                    printf(" %d}\n", t[tmp].num);
                    new_set = false;
                    continue;
                } 
                i++;
            }
        } else if (t[0].kind == Character) {
            if (!is_in_table(t[0].value, sets)) {
                fprintf(stderr, "%s%c must be defined\n", buff, t[0].value);
                fprintf(stderr, "%s   | try -> %c = {...}\n", buff, t[0].value);
                continue;
            } else if (t[1].kind == End) {
                printf("%s", buff);
                print_set(get_hash_from_table(t[0].value, sets).set);
            } else if (!is_func(t[1].kind)) {
                char format = (t[2].kind == Character) ? t[2].value : 'x';
                fprintf(stderr, "%sExpected a binary operation\n", buff);
                fprintf(stderr, "%s   | try -> %c intersect %c \n", buff, t[0].value, format);
                fprintf(stderr, "%s   | try -> %c union %c     \n", buff, t[0].value, format);
            } else if (t[2].kind != Character) {
                fprintf(stderr, "%sExpected a set             \n", buff);
                fprintf(stderr, "%s   | try -> %c %s x        \n", buff, t[0].value, tokens[t[1].kind]);
            } else if (!is_in_table(t[2].value, sets)) {
                fprintf(stderr, "%s%c must be defined\n", buff, t[2].value);
                fprintf(stderr, "%s   | try -> %c = {...}     \n", buff, t[2].value);
            } else if (t[3].kind != End) {
                fprintf(stderr, "%sExpression is already complete\n", buff);
                fprintf(stderr, "%s   | try -> %c %s %c          \n", buff, t[0].value, tokens[t[1].kind], t[2].value);
            } else {
                if (t[1].kind == Union     ) {print_set(union_(get_hash_from_table(t[0].value, sets).set, get_hash_from_table(t[2].value, sets).set));}
                if (t[1].kind == Intersect ) {print_set(intersect(get_hash_from_table(t[0].value, sets).set, get_hash_from_table(t[2].value, sets).set));}
                if (t[1].kind == Compliment) {print_set(compliment(get_hash_from_table(t[0].value, sets).set, get_hash_from_table(t[2].value, sets).set));}
                if (t[1].kind == SubSet    ) {print_bool(subset(get_hash_from_table(t[0].value, sets).set, get_hash_from_table(t[2].value, sets).set));}
            }
            continue;
        } else if (t[0].kind == Pipe) {
            if (t[1].kind != Character) {
                fprintf(stderr, "%sExpected a set\n", buff);
                fprintf(stderr, "%s  | try -> |x|\n", buff);
                continue;
            } else if (!is_in_table(t[1].value, sets)) {
                fprintf(stderr, "%s%c must be defined\n", buff, t[1].value);
                fprintf(stderr, "%s   | try -> %c = {...}\n", buff, t[1].value);                
                continue;
            } else if (t[2].kind != Pipe) {
                fprintf(stderr, "%sExpected a closing pipe\n", buff);  
                fprintf(stderr, "%s   | try -> |%c|       \n", buff, t[1].value);  
                continue;
            } else if (t[3].kind != End) {
                fprintf(stderr, "%sExpression is already complete\n", buff);  
                fprintf(stderr, "%s   | try -> |%c|       \n", buff, t[1].value);
                continue;
            }
            printf("%s%d\n", buff, get_hash_from_table(t[1].value, sets).set.len);
        } else {
            fprintf(stderr, "not implemented yet\n" );
            continue;
        }
        
        if (new_set) {
            Hash h;
            Set tmp;
            tmp.len = 0;
            h.name = t[0].value;
                
            int i = 3;
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
            continue;
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
