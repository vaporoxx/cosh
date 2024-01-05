#pragma once

#include <stddef.h>

enum TokenType {
	TT_IDENTIFIER,
	TT_INTEGER,
	TT_OPERATOR,
	TT_SEPARATOR,
};

typedef enum TokenType TokenType;

struct Token {
	size_t index;
	TokenType type;
	char *value;
};

typedef struct Token Token;

struct Tokens {
	Token *elements;
	char *input;
	size_t size;
};

typedef struct Tokens Tokens;

void add_token(Tokens *tokens, size_t index, size_t length, TokenType type);

void free_tokens(Tokens *tokens);

Tokens new_tokens(char *input);
