#include "lexer.h"
#include <ctype.h>
#include <string.h>

int lex(char *input, Tokens *tokens, size_t *failed) {
	size_t index = 0;

	while (input[index]) {
		char value = input[index];

		if (isspace(value)) {
			index += 1;
			continue;
		}

		if (isalpha(value)) {
			size_t length = 1;

			while (isalpha(input[index + length])) {
				length += 1;
			}

			if (add_token(tokens, index, TT_IDENTIFIER, input, length)) {
				*failed = index;
				return 1;
			}

			index += length;
			continue;
		}

		if (isdigit(value)) {
			size_t length = 1;

			while (isdigit(input[index + length])) {
				length += 1;
			}

			if (add_token(tokens, index, TT_INTEGER, input, length)) {
				*failed = index;
				return 1;
			}

			index += length;
			continue;
		}

		if (strchr("%*+-/^", value)) {
			if (add_token(tokens, index, TT_OPERATOR, input, 1)) {
				*failed = index;
				return 1;
			}

			index += 1;
			continue;
		}

		if (strchr("()|", value)) {
			if (add_token(tokens, index, TT_SEPARATOR, input, 1)) {
				*failed = index;
				return 1;
			}

			index += 1;
			continue;
		}

		*failed = index;
		return 1;
	}

	if (add_token(tokens, index, TT_END, input, 0)) {
		*failed = index;
		return 1;
	}

	return 0;
}
