#include "parser.h"
#include <string.h>

static int parse_part(Tokens *tokens, Node **node, size_t *failed, size_t *index, char stop) {
	Node *root = NULL;
	int stopped = 0;

	while (*index < tokens->size) {
		Node *next = NULL;
		Token *token = tokens->elements + *index;

		*index += 1;

		if (token->type == TT_IDENTIFIER) {
			next = new_node(token->index, NT_VARIABLE, token->value);
		}

		if (token->type == TT_INTEGER) {
			next = new_node(token->index, NT_INTEGER, token->value);
		}

		if (token->type == TT_OPERATOR) {
			next = new_node(token->index, NT_OPERATOR, token->value);
		}

		if (token->type == TT_SEPARATOR) {
			if (token->value[0] == '(') {
				if (parse_part(tokens, &next, failed, index, ')')) {
					free_node(root);
					return 1;
				}

				next->grouped = 1;
			}

			if (token->value[0] == ')') {
				if (stop == ')') {
					stopped = 1;
					break;
				}

				free_node(root);

				*failed = token->index;
				return 1;
			}

			if (token->value[0] == '|') {
				if (stop == '|') {
					stopped = 1;
					break;
				}

				if (parse_part(tokens, &next, failed, index, '|')) {
					free_node(root);
					return 1;
				}

				Node *function = new_node(token->index, NT_FUNCTION, "abs");

				append_node(next, function);
				next = function;
			}
		}

		if (!root) {
			if (is_expression(next)) {
				root = next;
				continue;
			}

			free_node(next);

			*failed = token->index;
			return 1;
		}

		if (is_expression(root) && next->type == NT_OPERATOR && !next->right) {
			Node **child = &root;

			while (!(*child)->grouped && precedence(*child) < precedence(next)) {
				child = &(*child)->right;
			}

			append_node(*child, next);

			*child = next;
			continue;
		}

		if (root->type == NT_OPERATOR && !last_operator(root)->right && is_expression(next)) {
			append_node(next, last_operator(root));
			continue;
		}

		Node *rightest = root;

		while (rightest->right) {
			rightest = rightest->right;
		}

		if (rightest->type == NT_VARIABLE && !rightest->grouped) {
			rightest->type = NT_FUNCTION;
			append_node(next, rightest);

			continue;
		}

		free_node(next);
		free_node(root);

		*failed = token->index;
		return 1;
	}

	if (!is_expression(root) && stop && stopped) {
		free_node(root);

		*failed = tokens->elements[*index - 1].index;
		return 1;
	}

	if ((root && !is_expression(root)) || (stop && !stopped)) {
		free_node(root);

		*failed = strlen(tokens->input);
		return 1;
	}

	*node = root;
	return 0;
}

int parse(Tokens *tokens, Node **node, size_t *failed) {
	size_t index = 0;
	return parse_part(tokens, node, failed, &index, 0);
}
