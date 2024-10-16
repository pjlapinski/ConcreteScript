#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

void pushToken(TokenList *list, size_t *cap, Token token) {
    if (list->length >= *cap) {
        *cap *= 2;
        list->tokens = CSI_REALLOC(list->tokens, *cap * sizeof(Token));
    }
    list->tokens[list->length++] = token;
}

TokenList lex(const char *input) {
    size_t listCapacity = 16;
    TokenList tokens = {
        .tokens = CSI_MALLOC(sizeof(Token) * listCapacity),
        .length = 0
    };

    const char *it = input;
    while (it && *it != '\0') {
        if (*it == '\0') break;

        if (isspace(*it)) {
            ++it;
            continue;
        }

        if (*it == '.') {
            Token token = { .type = TOKEN_DOT };
            pushToken(&tokens, &listCapacity, token);
            ++it;
            continue;
        }

        if (isdigit(*it)) {
            Token token;
            const char *end = it;
            bool dot = false;
            while (isdigit(*end)) {
                ++end;
                if (*end == '.' && !dot) {
                    dot = true;
                    ++end;
                    if (!end || !isdigit(*end)) {
                        --end;
                        dot = false;
                    }
                }
            }
            String str = {
                .text = CSI_MALLOC(sizeof(char) * (end - it)),
                .length = end - it,
            };
            memcpy(str.text, it, end - it);
            if (dot) {
                token.type = TOKEN_FLOAT_LITERAL;
                token.data.stringValue = str;
            } else {
                token.type = TOKEN_INT_LITERAL;
                token.data.stringValue = str;
            }
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
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
            pushToken(&tokens, &listCapacity, token);
            continue;
        }

        if (*it == '\'') {
            // TODO: escape characters
            Token token;
            token.type = TOKEN_CHAR_LITERAL;
            const char *end = ++it;
            while (end && *end != '\'') {
                ++end;
            }
            if (!end || *end != '\'') {
                // TODO: report error
                break;
            }
            String str = {
                .text = CSI_MALLOC(sizeof(char) * (end - it)),
                .length = end - it,
            };
            memcpy(str.text, it, end - it);
            str.length = end - it;
            token.data.stringValue = str;
            pushToken(&tokens, &listCapacity, token);
            it = end + 1;
        }

        if (*it == '"') {
            // TODO: escape characters
            Token token;
            token.type = TOKEN_STRING_LITERAL;
            const char *end = ++it;
            while (end && *end != '"') {
                ++end;
            }
            if (!end || *end != '"') {
                // TODO: report error
                break;
            }
            String str = {
                .text = CSI_MALLOC(sizeof(char) * (end - it)),
                .length = end - it,
            };
            memcpy(str.text, it, end - it);
            str.length = end - it;
            token.data.stringValue = str;
            pushToken(&tokens, &listCapacity, token);
            it = end + 1;
        }

        if (*it == '(') {
            Token token = { .type = TOKEN_BRACKET_OPEN };
            pushToken(&tokens, &listCapacity, token);
            ++it;
            continue;
        }

        if (*it == ')') {
            Token token = { .type = TOKEN_BRACKET_OPEN };
            pushToken(&tokens, &listCapacity, token);
            ++it;
            continue;
        }

        if (isalpha(*it) || *it == '_') {
            Token token;
            token.type = TOKEN_IDENTIFIER;
            const char *end = it + 1;
            while (end && *end != '\0' && (isalnum(*end) || *end == '_')) {
                ++end;
            }
            if (!end) { --end; }
            String str = {
                .text = CSI_MALLOC(sizeof(char) * (end - it)),
                .length = end - it,
            };
            memcpy(str.text, it, end - it);
            token.data.stringValue = str;
            pushToken(&tokens, &listCapacity, token);
            it = end;
            continue;
        }
    }
    return tokens;
}

void freeTokens(TokenList tokens) {
    for (size_t i = 0; i < tokens.length; ++i) {
        if (tokens.tokens[i].type == TOKEN_IDENTIFIER ||
            tokens.tokens[i].type == TOKEN_STRING_LITERAL ||
            tokens.tokens[i].type == TOKEN_CHAR_LITERAL ||
            tokens.tokens[i].type == TOKEN_INT_LITERAL ||
            tokens.tokens[i].type == TOKEN_FLOAT_LITERAL) {
            CSI_FREE(tokens.tokens[i].data.stringValue.text);
        }
    }
    CSI_FREE(tokens.tokens);
    tokens.length = 0;
}

void printString(String str, size_t maxChars) {
    for (int i = 0; i < str.length; ++i) {
        if (maxChars != 0 && i >= maxChars) {
            printf("...");
            return;
        }
        printf("%c", str.text[i]);
    }
}

void debugPrintTokens(TokenList tokens) {
    for (size_t i = 0; i < tokens.length; ++i) {
        Token token = tokens.tokens[i];
        if (token.type == TOKEN_IDENTIFIER) {
            printf("TOKEN_IDENTIFIER[");
            printString(token.data.stringValue, 0);
            printf("]\n");
        } else if (token.type == TOKEN_BRACKET_OPEN) {
            printf("TOKEN_BRACKET_OPEN\n");
        } else if (token.type == TOKEN_BRACKET_CLOSE) {
            printf("TOKEN_BRACKET_CLOSE\n");
        } else if (token.type == TOKEN_STRING_LITERAL) {
            printf("TOKEN_STRING_LITERAL[");
            printString(token.data.stringValue, 10);
            printf("]\n");
        } else if (token.type == TOKEN_CHAR_LITERAL) {
            printf("TOKEN_CHAR_LITERAL[");
            printString(token.data.stringValue, 0);
            printf("]\n");
        } else if (token.type == TOKEN_INT_LITERAL) {
            printf("TOKEN_INT_LITERAL[");
            printString(token.data.stringValue, 0);
            printf("]\n");
        } else if (token.type == TOKEN_FLOAT_LITERAL) {
            printf("TOKEN_FLOAT_LITERAL[");
            printString(token.data.stringValue, 0);
            printf("]\n");
        } else if (token.type == TOKEN_DOT) {
            printf("TOKEN_DOT\n");
        } else if (token.type == TOKEN_OPERATOR) {
            printf("TOKEN_OPERATOR[");
            if (token.data.operator == OPERATOR_PLUS) {
                printf("OPERATOR_PLUS");
            } else if (token.data.operator == OPERATOR_PLUS_EQ) {
                printf("OPERATOR_PLUS_EQ");
            } else if (token.data.operator == OPERATOR_MINUS) {
                printf("OPERATOR_MINUS");
            } else if (token.data.operator == OPERATOR_MINUS_EQ) {
                printf("OPERATOR_MINUS_EQ");
            } else if (token.data.operator == OPERATOR_MUL) {
                printf("OPERATOR_MUL");
            } else if (token.data.operator == OPERATOR_MUL_EQ) {
                printf("OPERATOR_MUL_EQ");
            } else if (token.data.operator == OPERATOR_DIV) {
                printf("OPERATOR_DIV");
            } else if (token.data.operator == OPERATOR_DIV_EQ) {
                printf("OPERATOR_DIV_EQ");
            } else if (token.data.operator == OPERATOR_ASSIGN) {
                printf("OPERATOR_ASSIGN");
            } else if (token.data.operator == OPERATOR_EQ) {
                printf("OPERATOR_EQ");
            } else if (token.data.operator == OPERATOR_NEQ) {
                printf("OPERATOR_NEQ");
            } else if (token.data.operator == OPERATOR_LT) {
                printf("OPERATOR_LT");
            } else if (token.data.operator == OPERATOR_GT) {
                printf("OPERATOR_GT");
            } else if (token.data.operator == OPERATOR_LE) {
                printf("OPERATOR_LE");
            } else if (token.data.operator == OPERATOR_GE) {
                printf("OPERATOR_GE");
            } else if (token.data.operator == OPERATOR_NOT) {
                printf("OPERATOR_NOT");
            }
            printf("]\n");
        }
    }
}
