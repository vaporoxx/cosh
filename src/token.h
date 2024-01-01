#pragma once

#include <stddef.h>

enum TokenType {
	TT_END,
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
	size_t size;
};

typedef struct Tokens Tokens;

int add_token(Tokens *tokens, size_t index, TokenType type, char *input, size_t length);

void free_tokens(Tokens *tokens);

Tokens new_tokens(void);
