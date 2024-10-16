#include "lexer.h"

int main(int argc, char **argv) {
    TokenList tokens = lex(
        "+ += - -= * *= / /= = == != > >= < <= ! var var_x varY _ () \"hello world\" 'a' 2137 21.37 21."
    );
    debugPrintTokens(tokens);
    freeTokens(tokens);
    return 0;
}
