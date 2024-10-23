#pragma once

#include <stddef.h>

static const char comment_symbol = ';';

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSE,
    TOKEN_CURLBRACE_OPEN,
    TOKEN_CURLBRACE_CLOSE,
    TOKEN_SQBRACKET_CLOSE,
    TOKEN_SQBRACKET_OPEN,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_DOT,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,

    TOKEN__MAX
} TokenType;

static const char *const token_names[] = {
    [TOKEN_IDENTIFIER] = "TOKEN_IDENTIFIER",
    [TOKEN_BRACKET_OPEN] = "TOKEN_BRACKET_OPEN",
    [TOKEN_BRACKET_CLOSE] = "TOKEN_BRACKET_CLOSE",
    [TOKEN_CURLBRACE_OPEN] = "TOKEN_CURLBRACE_OPEN",
    [TOKEN_CURLBRACE_CLOSE] = "TOKEN_CURLBRACE_CLOSE",
    [TOKEN_SQBRACKET_OPEN] = "TOKEN_SQBRACKET_OPEN",
    [TOKEN_SQBRACKET_CLOSE] = "TOKEN_SQBRACKET_CLOSE",
    [TOKEN_STRING_LITERAL] = "TOKEN_STRING_LITERAL",
    [TOKEN_CHAR_LITERAL] = "TOKEN_CHAR_LITERAL",
    [TOKEN_INT_LITERAL] = "TOKEN_INT_LITERAL",
    [TOKEN_FLOAT_LITERAL] = "TOKEN_FLOAT_LITERAL",
    [TOKEN_DOT] = "TOKEN_DOT",
    [TOKEN_OPERATOR] = "TOKEN_OPERATOR",
    [TOKEN_KEYWORD] = "TOKEN_KEYWORD",
};
static_assert(sizeof(token_names) / sizeof(token_names[0]) == TOKEN__MAX,
    "Token enum and token_names list lengths are not equal");

typedef enum {
    KEYWORD_FN,
    KEYWORD_NOT,
    KEYWORD_AND,
    KEYWORD_OR,

    KEYWORD__MAX
} Keyword;

static const char *const keywords[] = {
    [KEYWORD_FN] = "fn",
    [KEYWORD_NOT] = "not",
    [KEYWORD_AND] = "and",
    [KEYWORD_OR] = "or"
};
static_assert(sizeof(keywords) / sizeof(keywords[0]) == KEYWORD__MAX,
    "Keyword enum and keyword_names list lengths are not equal");

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
    OPERATOR_XOR,
    OPERATOR_OR,
    OPERATOR_AND,
    OPERATOR_XOREQ,
    OPERATOR_OREQ,
    OPERATOR_ANDEQ,
    OPERATOR_SHIFTL,
    OPERATOR_SHIFTR,
    OPERATOR_SHIFTLEQ,
    OPERATOR_SHIFTREQ,

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
    [OPERATOR_XOR] = "OPERATOR_XOR",
    [OPERATOR_OR] = "OPERATOR_OR",
    [OPERATOR_AND] = "OPERATOR_AND",
    [OPERATOR_XOREQ] = "OPERATOR_XOREQ",
    [OPERATOR_OREQ] = "OPERATOR_OREQ",
    [OPERATOR_ANDEQ] = "OPERATOR_ANDEQ",
    [OPERATOR_SHIFTL] = "OPERATOR_SHIFTL",
    [OPERATOR_SHIFTR] = "OPERATOR_SHIFTR",
    [OPERATOR_SHIFTLEQ] = "OPERATOR_SHIFTLEQ",
    [OPERATOR_SHIFTREQ] = "OPERATOR_SHIFTREQ"
};
static_assert(sizeof(operator_names) / sizeof(operator_names[0]) == OPERATOR__MAX,
    "Operator enum and operator_names list lengths are not equal");

typedef struct {
    char *text;
    size_t length;
} String;

typedef union {
    String string_value;
    Operator operator;
    Keyword keyword;
} TokenData;

typedef struct {
    TokenType type;
    TokenData data;
    size_t col, row;
} Token;

typedef struct {
    Token *tokens;
    size_t length;
} TokenList;

TokenList lex(const char *);
void free_tokens(TokenList *);
void debug_print_tokens(TokenList);
bool streq(String *, String *);
bool streqc(String *, const char *);
