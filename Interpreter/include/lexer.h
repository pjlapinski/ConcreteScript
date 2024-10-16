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
    TOKEN_OPERATOR
} TokenType;

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
    OPERATOR_NOT
} Operator;

typedef struct {
    char *text;
    size_t length;
} String;

typedef union {
    String stringValue;
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
void freeTokens(TokenList tokens);
void debugPrintTokens(TokenList tokens);
