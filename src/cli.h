#pragma once

#include "token.h"

struct Args {
	int debug;
	int help;
	char *input;
};

typedef struct Args Args;

void error_lexer(char *input, size_t index);

void error_parser(Token *token);

char *get_input(void);

Args parse_args(int argc, char **argv);
