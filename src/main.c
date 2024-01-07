#include "cli.h"
#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static int run_input(Args *args, char *input) {
	size_t failed = 0;
	Node *node = NULL;
	Tokens tokens = new_tokens(input);

	if (lex(input, &tokens, &failed)) {
		error("lexer", input, failed);

		free_tokens(&tokens);
		return 1;
	}

	if (parse(&tokens, &node, &failed)) {
		error("parser", input, failed);

		free_tokens(&tokens);
		return 1;
	}

	if (args->debug) {
		debug(node);
	} else {
		puts("0");
	}

	free_node(node);
	free_tokens(&tokens);

	return 0;
}

int main(int argc, char **argv) {
	Args args = parse_args(argc, argv);

	if (args.help) {
		puts("cosh: interactive math shell\n");
		puts("Usage: cosh [-d] [-h] [-i INPUT]");
		puts("  -d, --debug    Prints the AST of the input instead of running it");
		puts("  -h, --help     Prints this help message");
		puts("  -i, --input    Runs INPUT instead of being interactive");

		return 0;
	}

	if (args.input) {
		return run_input(&args, args.input);
	}

	fputs("cosh: interactive math shell\n", stderr);

	while (!feof(stdin)) {
		fputs("\n>>> ", stderr);
		fflush(stderr);

		char *input = get_input();

		if (feof(stdin)) {
			fputs("^D\n", stderr);
		} else {
			run_input(&args, input);
		}

		free(input);
	}

	return 0;
}
