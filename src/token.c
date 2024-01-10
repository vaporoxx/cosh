#include "token.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>

void add_token(Tokens *tokens, size_t index, char *input, size_t length, TokenType type) {
	char *value = xmalloc(length + 1);

	memcpy(value, input + index, length);
	value[length] = 0;

	tokens->elements = xrealloc(tokens->elements, sizeof(Token) * (tokens->size + 1));
	tokens->elements[tokens->size] = (Token){index, type, value};
	tokens->size += 1;
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
