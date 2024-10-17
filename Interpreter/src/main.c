#include <stdio.h>
#include <string.h>

#include "config.h"
#include "lexer.h"

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "rb");
    fseek(input, 0, SEEK_END);
    size_t len = ftell(input);
    fseek(input, 0, SEEK_SET);
    char *program = CSI_MALLOC(len + 1);
    fread(program, len, 1, input);
    program[len] = '\0';
    fclose(input);

    TokenList tokens = lex(program);
    CSI_FREE(program);

    debug_print_tokens(tokens);
    free_tokens(&tokens);
    return 0;
}
