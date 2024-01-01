#include "token.h"
#include <stdlib.h>
#include <string.h>

int add_token(Tokens *tokens, size_t index, TokenType type, char *input, size_t length) {
	char *value = malloc(length + 1);

	if (!value) {
		return 1;
	}

	Token *elements = realloc(tokens->elements, sizeof(Token) * (tokens->size + 1));

	if (!elements) {
		free(value);
		return 1;
	}

	memcpy(value, input + index, length);
	value[length] = 0;

	tokens->elements = elements;
	tokens->elements[tokens->size] = (Token){index, type, value};
	tokens->size += 1;

	return 0;
}

void free_tokens(Tokens *tokens) {
	for (size_t i = 0; i < tokens->size; i += 1) {
		free(tokens->elements[i].value);
	}

	free(tokens->elements);
}

Tokens new_tokens(void) {
	return (Tokens){NULL, 0};
}
