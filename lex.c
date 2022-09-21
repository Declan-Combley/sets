#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum TokenKind {
    // Special Characters
    OpenBracket,
    CloseBracket,
    OpenParenthesis,
    CloseParenthesis,
    ForwardSlash,
    Hyphon,
    Pipe,
    Empty,
    Comma,
    Chevron,
    Eq,

    // Types 
    Num,
    Character,
    String,

    // Key Words
    Not,
    Compliment,
    Union,
    Intersect,
    SubSet,
    Of,

    Err,
    End,
} TokenKind;

char tokens[][20] = {
    "OpenBracket",
    "CloseBracket",
    "OpenParenthesis",
    "CloseParenthesis",
    "ForwardSlash",
    "Hyphon",
    "Pipe",
    "Empty",
    "Comma",
    "Chevron",
    "Eq",
    "Num",
    "Character",
    "String",
    "Not",
    "Compliment",
    "Union",
    "Intersect",
    "SubSet",
    "Of",
    "Err",
    "End"
};    

typedef struct Token {
    TokenKind kind;
    char string[255];
    char value;
    int num;
} Token;

Token to_token(char t)
{
    Token token;

    if      (t == '{')        {token.kind = OpenBracket;     }
    else if (t == '}')        {token.kind = CloseBracket;    }
    else if (t == '(')        {token.kind = OpenParenthesis; }
    else if (t == ')')        {token.kind = CloseParenthesis;}
    else if (t == ',')        {token.kind = Comma;           }
    else if (t == ' ')        {token.kind = Empty;           }
    else if (t == '|')        {token.kind = Pipe;            }
    else if (t == '=')        {token.kind = Eq;              }
    else if (t == '^')        {token.kind = Chevron;         }
    else if (t == '\\')       {token.kind = ForwardSlash;    }
    else if (isdigit(t) == 0) {token.kind = Character;       }
    else if (isdigit(t) != 0) {
        token.kind = Num;
        token.value = t;
        token.num = t - '0';
        return token;
    } else                    {token.kind = Err;             }

    token.value = t;
    token.num = 0;

    return token;
}

void print_token(Token t)
{
    if      (t.kind == Character) { printf("{ %s : '%c' } ", tokens[t.kind], t.value);}
    else if (t.kind == String)    { printf("{ %s : %s } ", tokens[t.kind], t.string);  } 
    else if (t.kind == Num)       { printf("{ %s : %d } ", tokens[t.kind], t.num);  } 
    else                          { printf("{ %s } ", tokens[t.kind]);         }
}

void print_tokens(Token *t)
{
    int i = 0;
    while (t[i + 1].kind != End) {
        print_token(t[i]);
        i++;
    }
    print_token(t[i++]);
    printf("\n");
}

Token *tokenize(char *s)
{
    size_t len = strlen(s);
    size_t counter = 0;
    
    Token tmp_tokens[len + 1];
    Token *tokens = malloc(255 * sizeof(Token));

    for (size_t i = 0; i < len; i++) {
        tmp_tokens[i] = to_token(s[i]);
    }
    
    tmp_tokens[len].value = End;
    tmp_tokens[len].kind = End;

    int i = 0;
    while (tmp_tokens[i].kind != End) {
        if (tmp_tokens[i].kind == Num && tmp_tokens[i + 1].kind == Num) {
            char s[255] = "";
            while (tmp_tokens[i].kind == Num) {
                strncat(s, &tmp_tokens[i].value, 1);
                tokens[counter].kind = Num;
                tokens[counter].num = atoi(s);
                tokens[counter].value = tmp_tokens[i].value;
                i++;
            }
        }
        else if (tmp_tokens[i].kind == Character && tmp_tokens[i + 1].kind == Character) {
            char s[255] = "";
            while (tmp_tokens[i].kind == Character) {
                strncat(s, &tmp_tokens[i].value, 1);
                i++;
            }
            
            if      (strcmp(s, "union"    ) == 0) { tokens[counter].kind = Union;     }
            else if (strcmp(s, "not"      ) == 0) { tokens[counter].kind = Not;       }
            else if (strcmp(s, "intersect") == 0) { tokens[counter].kind = Intersect; }
            else if (strcmp(s, "subset"   ) == 0) { tokens[counter].kind = SubSet;    }
            else if (strcmp(s, "of"       ) == 0) { tokens[counter].kind = Of;        }
            else {
                strcpy(tokens[counter].string, s);
                tokens[counter].kind = String;
                tokens[counter].value = ' ';
            }
            tokens[counter].num = 0;
            tokens[counter].value = 'f';
        }
        else if (tmp_tokens[i].kind == Empty) {
            counter--;
            i++;
        } else {
            tokens[counter] = tmp_tokens[i];
            i++;
        }
        counter++;
    }
    
    tokens[counter].value = End;
    tokens[counter].kind = End;

    return tokens;
}
