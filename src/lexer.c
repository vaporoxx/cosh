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

		if (value == '=') {
			add_token(tokens, index, input, 1, TT_ASSIGNMENT);

			index += 1;
			continue;
		}

		if (isalpha(value)) {
			size_t length = 1;

			while (isalpha(input[index + length])) {
				length += 1;
			}

			add_token(tokens, index, input, length, TT_IDENTIFIER);

			index += length;
			continue;
		}

		if (isdigit(value)) {
			size_t length = 1;

			while (isdigit(input[index + length])) {
				length += 1;
			}

			if (input[index + length] && strchr(",.", input[index + length])) {
				length += 1;

				while (isdigit(input[index + length])) {
					length += 1;
				}
			}

			add_token(tokens, index, input, length, TT_LITERAL);

			index += length;
			continue;
		}

		if (strchr("%*+-/^", value)) {
			add_token(tokens, index, input, 1, TT_OPERATOR);

			index += 1;
			continue;
		}

		if (strchr("()|", value)) {
			add_token(tokens, index, input, 1, TT_SEPARATOR);

			index += 1;
			continue;
		}

		*failed = index;
		return 1;
	}

	return 0;
}
