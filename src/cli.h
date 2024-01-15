#pragma once

#include "node.h"
#include "token.h"

struct Args {
	int debug;
	int help;
	char *input;
};

typedef struct Args Args;

void error_lexer(char *input, size_t index);

void error_parser(Token *token);

void error_runner(Node *node, char *message);

char *get_input(void);

Args new_args(void);

int parse_args(int argc, char **argv, Args *args, char **failed);
