#include "lexer.h"

int main(int argc, char **argv) {
    TokenList tokens = lex(
        "+ += - -= * *= / /= = == != > >= < <= ! var var_x varY _ () \"hello world\" 'a' 2137 21.37 21."
    );
    debug_print_tokens(tokens);
    free_tokens(tokens);
    return 0;
}
