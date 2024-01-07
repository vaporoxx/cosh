#pragma once

#include <stddef.h>

struct Args {
	int debug;
	int help;
	char *input;
};

typedef struct Args Args;

void error(char *location, char *input, size_t failed);

char *get_input(void);

Args parse_args(int argc, char **argv);
