#include "lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"

static size_t listCapacity, listLength;

void pushToken(Token *list, Token token) {
    if (listLength >= listCapacity) {
        listCapacity *= 2;
        list = CSI_REALLOC(list, listCapacity * sizeof(Token));
    }
    list[listLength++] = token;
}

void lex(const char *input, Token *outTokens) {
    listCapacity = 16;
    listLength = 0;
    outTokens = CSI_MALLOC(sizeof(Token) * listCapacity);

    char *it = input;
    while (it) {
        if (*it == '\0') break;

        if (isspace(*it)) {
            ++it;
            continue;
        }

        if (*it == '.') {
            Token token { .type = TOKEN_DOT };
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (isdigit(*it)) {
            Token token;
            char *end = it;
            bool dot = false;
            while (isdigit(*end)) {
                ++end;
                if (*end == '.' && !dot) {
                    dot = true;
                    ++end;
                    if (!isdigit(*end)) {
                        --end;
                        dot = false;
                    }
                }
            }
            char *str = NULL;
            memcpy(str, it, end - it);
            if (dot) {
                token.type = TOKEN_FLOAT_LITERAL;
                token.data.floatValue = atof(str);
            } else {
                token.type = TOKEN_INT_LITERAL;
                token.data.intValue = atoll(str);
            }
            pushToken(outTokens, token);
            it = end;
            continue;
        }

        if (*it == '+') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_PLUS_EQ;
                ++it;
            } else {
                token.data.operator = OPERATOR_PLUS;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '-') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_MINUS_EQ;
                ++it;
            } else {
                token.data.operator = OPERATOR_MINUS;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '*') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_MUL_EQ;
                ++it;
            } else {
                token.data.operator = OPERATOR_MUL;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '/') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_DIV_EQ;
                ++it;
            } else {
                token.data.operator = OPERATOR_DIV;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '=') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_EQ;
                ++it;
            } else {
                token.data.operator = OPERATOR_ASSIGN;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '!') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_NEQ;
                ++it;
            } else {
                token.data.operator = OPERATOR_NOT;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '>') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_GE;
                ++it;
            } else {
                token.data.operator = OPERATOR_GT;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '<') {
            Token token;
            token.type = TOKEN_OPERATOR;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_LE;
                ++it;
            } else {
                token.data.operator = OPERATOR_LT;
            }
            pushToken(outTokens, token);
            ++it;
            continue;
        }

        if (*it == '\'') {
            Token token;
            token.type = TOKEN_CHAR_LITERAL;
        }

        if (*it == '"') {
            Token token;
            token.type = TOKEN_STRING_LITERAL;
        }
    }
}
