#include "cli.h"
#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include "runner.h"
#include <stdio.h>
#include <stdlib.h>

static int run_input(Args *args, char *input) {
	Node *node = NULL;
	Tokens tokens = new_tokens();

	size_t failed_index = 0;
	Node *failed_node = NULL;
	Token *failed_token = NULL;
	char *message = NULL;

	if (lex(input, &tokens, &failed_index)) {
		error_lexer(input, failed_index);

		if (!args->input) {
			fputc('\n', stderr);
		}

		free_tokens(&tokens);
		return 1;
	}

	if (parse(&tokens, &node, &failed_token)) {
		error_parser(failed_token);

		if (!args->input) {
			fputc('\n', stderr);
		}

		free_tokens(&tokens);
		return 1;
	}

	if (args->debug) {
		debug(node);
	} else if (run(node, &failed_node, &message)) {
		error_runner(failed_node, message);

		if (!args->input) {
			fputc('\n', stderr);
		}

		free_node(node);
		free_tokens(&tokens);

		return 1;
	}

	if (!args->input && node) {
		fputc('\n', stderr);
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

	fputs("cosh: interactive math shell\n\n", stderr);

	while (!feof(stdin)) {
		fputs(">>> ", stderr);
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
