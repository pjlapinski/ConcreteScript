#include "lexer.h"
#include "config.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

void push_token(TokenList *list, size_t *cap, Token token) {
    if (list->length >= *cap) {
        *cap *= 2;
        list->tokens = CSI_REALLOC(list->tokens, *cap * sizeof(Token));
    }
    list->tokens[list->length++] = token;
}

void err_print_until_new_line(const char *line) {
    const char *p = line;
    while (*p && *p != '\n') {
        fprintf(stderr, "%c", *p);
        ++p;
    }
    fprintf(stderr, "\n");
}

TokenList lex(const char *input) {
    size_t list_capacity = 16;
    TokenList tokens = {
        .tokens = CSI_MALLOC(sizeof(Token) * list_capacity),
        .length = 0
    };

    const char *it = input;
    size_t col = 1, row = 1;
    while (it && *it != '\0') {
        if (*it == '\0') break;

        if (*it == comment_symbol) {
            ++it;
            while (*it && *it != '\n') {
                ++it;
            }
            continue;
        }

        if (isspace(*it)) {
            ++col;
            if (*it == '\n') {
                col = 1;
                ++row;
            }
            ++it;
            continue;
        }

        if (*it == '.') {
            Token token = { .type = TOKEN_DOT, .col = col++, .row = row };
            push_token(&tokens, &list_capacity, token);
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
            token.col = col;
            token.row = row;
            col += str.length;
            push_token(&tokens, &list_capacity, token);
            it = end;
            continue;
        }

        if (*it == '+') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_PLUS_EQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_PLUS;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '-') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_MINUS_EQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_MINUS;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '*') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_MUL_EQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_MUL;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '/') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_DIV_EQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_DIV;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '=') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_EQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_ASSIGN;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '!') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_NEQ;
                ++it;
                ++col;
            } else {
                // TODO: report error, '!' doesn't mean anything
                break;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '>') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_GE;
                ++it;
                ++col;
            } else if (*it == '>') {
                token.data.operator = OPERATOR_SHIFTR;
                ++it;
                ++col;
                if (*it == '=') {
                    token.data.operator = OPERATOR_SHIFTREQ;
                    ++it;
                    ++col;
                }
            } else {
                token.data.operator = OPERATOR_GT;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '<') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_LE;
                ++it;
                ++col;
            } else if (*it == '<') {
                token.data.operator = OPERATOR_SHIFTL;
                ++it;
                ++col;
                if (*it == '=') {
                    token.data.operator = OPERATOR_SHIFTLEQ;
                    ++it;
                    ++col;
                }
            } else {
                token.data.operator = OPERATOR_LT;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '^') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_XOREQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_XOR;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '|') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_OREQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_OR;
            }
            push_token(&tokens, &list_capacity, token);
            continue;
        }

        if (*it == '&') {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.col = col++;
            token.row = row;
            ++it;
            if (*it == '=') {
                token.data.operator = OPERATOR_ANDEQ;
                ++it;
                ++col;
            } else {
                token.data.operator = OPERATOR_AND;
            }
            push_token(&tokens, &list_capacity, token);
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
                // TODO: report error, invalid literal
                break;
            }
            String str = {
                .text = CSI_MALLOC(sizeof(char) * (end - it)),
                .length = end - it,
            };
            memcpy(str.text, it, end - it);
            str.length = end - it;
            token.data.string_value = str;
            token.col = col;
            token.row = row;
            push_token(&tokens, &list_capacity, token);
            col += str.length + 2;
            it = end + 1;
            continue;
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
                // TODO: report error, invalid literal
                break;
            }
            String str = {
                .text = CSI_MALLOC(sizeof(char) * (end - it)),
                .length = end - it,
            };
            memcpy(str.text, it, end - it);
            str.length = end - it;
            token.data.string_value = str;
            token.col = col;
            token.row = row;
            push_token(&tokens, &list_capacity, token);
            col += str.length + 2;
            it = end + 1;
            continue;
        }

        if (*it == '(') {
            Token token = { .type = TOKEN_BRACKET_OPEN, .col = col++, .row = row };
            push_token(&tokens, &list_capacity, token);
            ++it;
            continue;
        }

        if (*it == ')') {
            Token token = { .type = TOKEN_BRACKET_CLOSE, .col = col++, .row = row };
            push_token(&tokens, &list_capacity, token);
            ++it;
            continue;
        }

        if (*it == '{') {
            Token token = { .type = TOKEN_CURLBRACE_OPEN, .col = col++, .row = row };
            push_token(&tokens, &list_capacity, token);
            ++it;
            continue;
        }

        if (*it == '}') {
            Token token = { .type = TOKEN_CURLBRACE_CLOSE, .col = col++, .row = row };
            push_token(&tokens, &list_capacity, token);
            ++it;
            continue;
        }

        if (*it == '[') {
            Token token = { .type = TOKEN_SQBRACKET_OPEN, .col = col++, .row = row };
            push_token(&tokens, &list_capacity, token);
            ++it;
            continue;
        }

        if (*it == ']') {
            Token token = { .type = TOKEN_SQBRACKET_CLOSE, .col = col++, .row = row };
            push_token(&tokens, &list_capacity, token);
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
            token.col = col;
            token.row = row;
            size_t i = 0;
            for (; i < KEYWORD__MAX; ++i) {
                if (streqc(&str, keywords[i])) {
                    token.type = TOKEN_KEYWORD;
                    token.data.keyword = (Keyword)i;
                    break;
                }
            }
            if (i == KEYWORD__MAX) {
                token.data.string_value = str;
            }
            push_token(&tokens, &list_capacity, token);
            col += str.length;
            it = end;
            continue;
        }

        fprintf(stderr, "Unknown identifier at [%zu:%zu]:\n", row, col);
        err_print_until_new_line(it);
        free_tokens(&tokens);
        return tokens;
    }
    return tokens;
}

void free_tokens(TokenList *tokens) {
    if (tokens->length == 0) {
        return;
    }
    for (size_t i = 0; i < tokens->length; ++i) {
        if (tokens->tokens[i].type == TOKEN_IDENTIFIER ||
            tokens->tokens[i].type == TOKEN_STRING_LITERAL ||
            tokens->tokens[i].type == TOKEN_CHAR_LITERAL ||
            tokens->tokens[i].type == TOKEN_INT_LITERAL ||
            tokens->tokens[i].type == TOKEN_FLOAT_LITERAL) {
            CSI_FREE(tokens->tokens[i].data.string_value.text);
        }
    }
    CSI_FREE(tokens->tokens);
    tokens->length = 0;
}

void print_string(String str, size_t max_chars) {
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
        printf("%zu:%zu - %s", token.row, token.col, token_names[token.type]);
        if (token.type == TOKEN_OPERATOR) {
            printf("[%s]\n", operator_names[token.data.operator]);
        } else if (token.type == TOKEN_KEYWORD) {
            printf("[%s]\n", keywords[token.data.keyword]);
        } else if (token.type == TOKEN_IDENTIFIER ||
            token.type == TOKEN_STRING_LITERAL ||
            token.type == TOKEN_CHAR_LITERAL ||
            token.type == TOKEN_INT_LITERAL ||
            token.type == TOKEN_FLOAT_LITERAL
            ) {
            printf("[");
            print_string(token.data.string_value, 0);
            printf("]\n");
        } else {
            printf("\n");
        }
    }
}

bool streq(String *a, String *b) {
    if (a->length != b->length) {
        return false;
    }
    for (size_t i = 0; i < a->length; ++i) {
        if (a->text[i] != b->text[i]) {
            return false;
        }
    }
    return true;
}

bool streqc(String *a, const char *b) {
    String s = { .text = (char *)b, .length = strlen(b) };
    return streq(a, &s);
}
