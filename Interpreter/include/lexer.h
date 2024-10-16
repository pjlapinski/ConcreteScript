#pragma once

#include <stddef.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSE,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_DOT,
    TOKEN_OPERATOR,

    TOKEN__MAX
} TokenType;

static const char *const token_names[] = {
    [TOKEN_IDENTIFIER] = "TOKEN_IDENTIFIER",
    [TOKEN_BRACKET_OPEN] = "TOKEN_BRACKET_OPEN",
    [TOKEN_BRACKET_CLOSE] = "TOKEN_BRACKET_CLOSE",
    [TOKEN_STRING_LITERAL] = "TOKEN_STRING_LITERAL",
    [TOKEN_CHAR_LITERAL] = "TOKEN_CHAR_LITERAL",
    [TOKEN_INT_LITERAL] = "TOKEN_INT_LITERAL",
    [TOKEN_FLOAT_LITERAL] = "TOKEN_FLOAT_LITERAL",
    [TOKEN_DOT] = "TOKEN_DOT",
    [TOKEN_OPERATOR] = "TOKEN_OPERATOR"
};
static_assert(sizeof(token_names)/sizeof(token_names[0]) == TOKEN__MAX,
    "Token enum and token_names list lengths are not equal");

typedef enum {
    OPERATOR_PLUS,
    OPERATOR_PLUS_EQ,
    OPERATOR_MINUS,
    OPERATOR_MINUS_EQ,
    OPERATOR_MUL,
    OPERATOR_MUL_EQ,
    OPERATOR_DIV,
    OPERATOR_DIV_EQ,
    OPERATOR_ASSIGN,
    OPERATOR_EQ,
    OPERATOR_NEQ,
    OPERATOR_LT,
    OPERATOR_GT,
    OPERATOR_LE,
    OPERATOR_GE,
    OPERATOR_NOT,

    OPERATOR__MAX
} Operator;

static const char *const operator_names[] = {
    [OPERATOR_PLUS] = "OPERATOR_PLUS",
    [OPERATOR_PLUS_EQ] = "OPERATOR_PLUS_EQ",
    [OPERATOR_MINUS] = "OPERATOR_MINUS",
    [OPERATOR_MINUS_EQ] = "OPERATOR_MINUS_EQ",
    [OPERATOR_MUL] = "OPERATOR_MUL",
    [OPERATOR_MUL_EQ] = "OPERATOR_MUL_EQ",
    [OPERATOR_DIV] = "OPERATOR_DIV",
    [OPERATOR_DIV_EQ] = "OPERATOR_DIV_EQ",
    [OPERATOR_ASSIGN] = "OPERATOR_ASSIGN",
    [OPERATOR_EQ] = "OPERATOR_EQ",
    [OPERATOR_NEQ] = "OPERATOR_NEQ",
    [OPERATOR_LT] = "OPERATOR_LT",
    [OPERATOR_GT] = "OPERATOR_GT",
    [OPERATOR_LE] = "OPERATOR_LE",
    [OPERATOR_GE] = "OPERATOR_GE",
    [OPERATOR_NOT] = "OPERATOR_NOT"
};
static_assert(sizeof(operator_names)/sizeof(operator_names[0]) == OPERATOR__MAX,
    "Operator enum and operator_names list lengths are not equal");

typedef struct {
    char *text;
    size_t length;
} String;

typedef union {
    String string_value;
    Operator operator;
} TokenData;

typedef struct {
    TokenType type;
    TokenData data;
} Token;

typedef struct {
    Token *tokens;
    size_t length;
} TokenList;

TokenList lex(const char *input);
void free_tokens(TokenList tokens);
void debug_print_tokens(TokenList tokens);
