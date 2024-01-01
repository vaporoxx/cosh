#include "cli.h"
#include <stdio.h>
#include <stdlib.h>

void error(char *location, char *input, size_t failed) {
	if (input[failed]) {
		fprintf(stderr, "cosh: %s error at '%c', column %zu\n", location, input[failed], failed + 1);
	} else {
		fprintf(stderr, "cosh: %s error at end of input\n", location);
	}
}

int get_input(char **input) {
	char *line = calloc(1, 1);
	size_t size = 1;

	if (!line) {
		return 1;
	}

	while (1) {
		int read = getchar();

		if (read == EOF || read == '\n') {
			*input = line;
			return 0;
		}

		char *new = realloc(line, size + 1);

		if (!new) {
			free(line);
			return 1;
		}

		line = new;
		line[size - 1] = read;
		line[size] = 0;

		size += 1;
	}
}
