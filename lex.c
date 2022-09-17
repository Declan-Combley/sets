#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum TokenKind {
    // Special Characters
    OpenBracket,
    CloseBracket,
    OpenParenthesis,
    CloseParenthesis,
    Empty,
    Comma,
    Num,
    Character,

    // Key Words
    Not,
    Union,
    Intersect,
    SubSet,
    Of,

    Err,
    End,
} TokenKind;

const char tokens[][17] = {
    "OpenBracket",
    "CloseBracket",
    "OpenParenthesis",
    "CloseParenthesis",
    "Empty",
    "Comma",
    "Num",
    "Character",
    "Not",
    "Union",
    "Intersect",
    "SubSet",
    "Of",
    "Err",
    "End"
};

typedef struct Token {
    TokenKind kind;
    char value;
    int num;
} Token;

void print_token(Token t)
{
    if      (t.kind == Character) { printf("{ %s : '%c' }\n", tokens[t.kind], t.value);}
    else if (t.kind == Num)       { printf("{ %s : %d }  \n", tokens[t.kind], t.num);  } 
    else                          { printf("{ %s }       \n", tokens[t.kind]);         }
}

Token to_token(char t)
{
    Token token;

    if      (t == '{')        {token.kind = OpenBracket;     }
    else if (t == '}')        {token.kind = CloseBracket;    }
    else if (t == '(')        {token.kind = OpenParenthesis; }
    else if (t == ')')        {token.kind = CloseParenthesis;}
    else if (t == ',')        {token.kind = Comma;           }
    else if (t == ' ')        {token.kind = Empty;           }
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

void print_tokens(Token *t)
{
    int i = 0;
    while (t[i].kind != End) {
        print_token(t[i]);
        i++;
    }    
}

Token *tokenize(char *s)
{
    size_t len = strlen(s);
    size_t counter = 0;
    
    Token tmp_tokens[len + 1];
    Token *tokens = malloc(len * sizeof(Token));

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
                tokens[counter].kind = Err;
                tokens[counter].value = Err;
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
