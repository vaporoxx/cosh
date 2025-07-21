#include "cli.h"

#include "memory.h"

#include <stdio.h>
#include <string.h>

static int is_option(char *option, char *short_name, char *long_name) {
	return !strcmp(option, short_name) || !strcmp(option, long_name);
}

void error_lexer(char *input, size_t index) {
	fprintf(stderr, "cosh: lexer error at '%c', column %zu\n", input[index], index + 1);
}

void error_parser(Token *token) {
	if (token) {
		fprintf(stderr, "cosh: parser error at '%s', column %zu\n", token->value, token->index + 1);
	} else {
		fputs("cosh: parser error at end of input\n", stderr);
	}
}

void error_runner(Node *node, char *message) {
	fprintf(stderr, "cosh: runner error at '%s', column %zu: %s\n", node->value, node->index + 1, message);
}

char *get_input(void) {
	char *input = xmalloc(1);
	size_t size = 1;

	input[0] = 0;

	while (1) {
		int read = getchar();

		if (read == EOF || read == '\n') {
			return input;
		}

		input = xrealloc(input, size + 1);
		input[size - 1] = read;
		input[size] = 0;

		size += 1;
	}
}

Args new_args(void) {
	return (Args){0, 0, NULL};
}

int parse_args(int argc, char **argv, Args *args, char **failed) {
	int in_option = 0;

	for (int i = 1; i < argc; i += 1) {
		char *last = argv[i - 1];
		char *next = argv[i];

		if (in_option && is_option(last, "-i", "--input")) {
			args->input = next;
			in_option = 0;

			continue;
		}

		if (is_option(next, "-d", "--debug")) {
			args->debug = 1;
			continue;
		}

		if (is_option(next, "-h", "--help")) {
			args->help = 1;
			continue;
		}

		if (is_option(next, "-i", "--input")) {
			in_option = 1;
			continue;
		}

		*failed = next;
		return 1;
	}

	if (in_option) {
		*failed = NULL;
		return 1;
	}

	return 0;
}
