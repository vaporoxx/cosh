#include "cli.h"
#include "memory.h"
#include <stdio.h>

void error(char *location, char *input, size_t failed) {
	if (input[failed]) {
		fprintf(stderr, "cosh: %s error at '%c', column %zu\n", location, input[failed], failed + 1);
	} else {
		fprintf(stderr, "cosh: %s error at end of input\n", location);
	}
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
