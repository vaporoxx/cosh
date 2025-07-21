#include "parser.h"

#include <string.h>

static int parse_part(Tokens *tokens, Node **node, Token **failed, size_t *index, char stop) {
	Node *root = NULL;
	int stopped = 0;

	while (*index < tokens->size) {
		Node *next = NULL;
		Token *token = tokens->elements + *index;
		char value = token->value[0];

		*index += 1;

		if (token->type == TT_ASSIGNMENT) {
			next = new_node(token->index, NT_ASSIGNMENT, token->value);
		}

		if (token->type == TT_IDENTIFIER) {
			next = new_node(token->index, NT_VARIABLE, token->value);
		}

		if (token->type == TT_LITERAL) {
			next = new_node(token->index, NT_LITERAL, token->value);
		}

		if (token->type == TT_OPERATOR) {
			next = new_node(token->index, NT_OPERATOR, token->value);
		}

		if (token->type == TT_SEPARATOR) {
			if (value == '(') {
				if (parse_part(tokens, &next, failed, index, ')')) {
					free_node(root);
					return 1;
				}

				next->grouped = 1;
			}

			if (value == ')') {
				if (stop == ')') {
					stopped = 1;
					break;
				}

				free_node(root);

				*failed = token;
				return 1;
			}

			if (value == '|') {
				if (stop == '|') {
					stopped = 1;
					break;
				}

				if (parse_part(tokens, &next, failed, index, '|')) {
					free_node(root);
					return 1;
				}

				Node *function = new_node(token->index, NT_FUNCTION, "abs");

				function->left = next;
				next = function;
			}
		}

		if (!root) {
			if (is_expression(next)) {
				root = next;
				continue;
			}

			if (strchr("+-", value)) {
				next->left = new_node(token->index, NT_LITERAL, "0");

				root = next;
				continue;
			}

			free_node(next);

			*failed = token;
			return 1;
		}

		if (root->type == NT_VARIABLE && next->type == NT_ASSIGNMENT && !stop) {
			next->left = root;

			if (parse_part(tokens, &next->right, failed, index, 0)) {
				free_node(next);
				return 1;
			}

			root = next;
			continue;
		}

		if (is_expression(root) && next->type == NT_OPERATOR && !next->right) {
			Node *child = root;

			while (!child->grouped && precedence(child) < precedence(next)) {
				child = child->right;
			}

			swap_nodes(child, next);

			child->left = next;
			continue;
		}

		if (is_expression(next)) {
			Node *rightest = root;

			while (rightest->right) {
				rightest = rightest->right;
			}

			if (rightest->type == NT_OPERATOR) {
				rightest->right = next;
				continue;
			}

			if (rightest->type == NT_VARIABLE && !rightest->grouped) {
				rightest->left = next;
				rightest->type = NT_FUNCTION;

				continue;
			}
		}

		free_node(next);
		free_node(root);

		*failed = token;
		return 1;
	}

	if (!is_expression(root) && stop && stopped) {
		free_node(root);

		*failed = tokens->elements + *index - 1;
		return 1;
	}

	if ((root && !is_expression(root)) || (stop && !stopped)) {
		free_node(root);

		*failed = NULL;
		return 1;
	}

	*node = root;
	return 0;
}

int parse(Tokens *tokens, Node **node, Token **failed) {
	size_t index = 0;
	return parse_part(tokens, node, failed, &index, 0);
}
