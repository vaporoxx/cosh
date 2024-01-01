#include "cli.h"
#include "lexer.h"
#include "parser.h"
#include "runner.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	fputs("cosh: interactive math shell\n", stderr);

	while (!feof(stdin)) {
		fputs("\n>>> ", stderr);
		fflush(stderr);

		char *input = NULL;

		if (get_input(&input)) {
			fputs("cosh: could not get input\n", stderr);
			return 1;
		}

		if (feof(stdin)) {
			fputs("^D\n", stderr);
		} else {
			size_t failed = 0;
			Node *node = NULL;
			Tokens tokens = new_tokens();

			if (lex(input, &tokens, &failed)) {
				error("lexer", input, failed);
			} else if (parse(&tokens, &node, &failed)) {
				error("parser", input, failed);
			} else if (run(node, &failed)) {
				error("runner", input, failed);
			}

			free_node(node);
			free_tokens(&tokens);
		}

		free(input);
	}

	return 0;
}
