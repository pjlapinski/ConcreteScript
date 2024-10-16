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
    size_t list_capacity = 16;
    TokenList tokens = {
        .tokens = CSI_MALLOC(sizeof(Token) * list_capacity),
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
            pushToken(&tokens, &list_capacity, token);
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
                token.data.string_value = str;
            } else {
                token.type = TOKEN_INT_LITERAL;
                token.data.string_value = str;
            }
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            pushToken(&tokens, &list_capacity, token);
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
            token.data.string_value = str;
            pushToken(&tokens, &list_capacity, token);
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
            token.data.string_value = str;
            pushToken(&tokens, &list_capacity, token);
            it = end + 1;
        }

        if (*it == '(') {
            Token token = { .type = TOKEN_BRACKET_OPEN };
            pushToken(&tokens, &list_capacity, token);
            ++it;
            continue;
        }

        if (*it == ')') {
            Token token = { .type = TOKEN_BRACKET_OPEN };
            pushToken(&tokens, &list_capacity, token);
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
            token.data.string_value = str;
            pushToken(&tokens, &list_capacity, token);
            it = end;
            continue;
        }
    }
    return tokens;
}

void free_tokens(TokenList tokens) {
    for (size_t i = 0; i < tokens.length; ++i) {
        if (tokens.tokens[i].type == TOKEN_IDENTIFIER ||
            tokens.tokens[i].type == TOKEN_STRING_LITERAL ||
            tokens.tokens[i].type == TOKEN_CHAR_LITERAL ||
            tokens.tokens[i].type == TOKEN_INT_LITERAL ||
            tokens.tokens[i].type == TOKEN_FLOAT_LITERAL) {
            CSI_FREE(tokens.tokens[i].data.string_value.text);
        }
    }
    CSI_FREE(tokens.tokens);
    tokens.length = 0;
}

void printString(String str, size_t max_chars) {
    for (int i = 0; i < str.length; ++i) {
        if (max_chars != 0 && i >= max_chars) {
            printf("...");
            return;
        }
        printf("%c", str.text[i]);
    }
}

void debug_print_tokens(TokenList tokens) {
    for (size_t i = 0; i < tokens.length; ++i) {
        Token token = tokens.tokens[i];
        printf("%s",  token_names[token.type]);
        if (token.type == TOKEN_OPERATOR) {
            printf("[%s]\n", operator_names[token.data.operator]);
        } else if (token.type == TOKEN_IDENTIFIER ||
            token.type == TOKEN_STRING_LITERAL ||
            token.type == TOKEN_CHAR_LITERAL ||
            token.type == TOKEN_INT_LITERAL ||
            token.type == TOKEN_FLOAT_LITERAL
            ) {
            printf("[");
            printString(token.data.string_value, 0);
            printf("]\n");
        } else {
            printf("\n");
        }
    }
}
