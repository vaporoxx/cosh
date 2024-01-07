#include "cli.h"
#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	fputs("cosh: interactive math shell\n", stderr);

	while (!feof(stdin)) {
		fputs("\n>>> ", stderr);
		fflush(stderr);

		char *input = get_input();

		if (feof(stdin)) {
			fputs("^D\n", stderr);
		} else {
			size_t failed = 0;
			Node *node = NULL;
			Tokens tokens = new_tokens(input);

			if (lex(input, &tokens, &failed)) {
				error("lexer", input, failed);
			} else if (parse(&tokens, &node, &failed)) {
				error("parser", input, failed);
			} else {
				debug(node);
			}

			free_node(node);
			free_tokens(&tokens);
		}

		free(input);
	}

	return 0;
}
