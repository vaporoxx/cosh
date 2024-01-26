#include "node.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>

void free_node(Node *node) {
	if (!node) {
		return;
	}

	free_node(node->left);
	free_node(node->right);
	free(node);
}

int is_child(Node *node, Node *parent) {
	if (!node || !parent) {
		return 0;
	}

	if (node == parent || is_child(node, parent->left) || is_child(node, parent->right)) {
		return 1;
	}

	return 0;
}

int is_expression(Node *node) {
	if (!node) {
		return 0;
	}

	switch (node->type) {
		case NT_ASSIGNMENT:
			return is_expression(node->left) && node->left->type == NT_VARIABLE && is_expression(node->right);
		case NT_FUNCTION:
			return is_expression(node->left) && !node->right;
		case NT_LITERAL:
			return !node->left && !node->right;
		case NT_OPERATOR:
			return is_expression(node->left) && is_expression(node->right);
		case NT_VARIABLE:
			return !node->left && !node->right;
	}

	return 0;
}

Node *new_node(size_t index, NodeType type, char *value) {
	Node *node = xmalloc(sizeof(Node));

	*node = (Node){0, index, NULL, NULL, type, value};
	return node;
}

char *node_type(NodeType type) {
	switch (type) {
		case NT_ASSIGNMENT:
			return "Assignment";
		case NT_FUNCTION:
			return "Function";
		case NT_LITERAL:
			return "Literal";
		case NT_OPERATOR:
			return "Operator";
		case NT_VARIABLE:
			return "Variable";
	}

	return "Unknown";
}

int precedence(Node *node) {
	if (!node) {
		return 0;
	}

	char value = node->value[0];

	if (strchr("+-", value)) {
		return 1;
	}

	if (strchr("%*/", value)) {
		return 2;
	}

	if (strchr("^", value)) {
		return 3;
	}

	return 4;
}

void swap_nodes(Node *first, Node *second) {
	if (!first || !second) {
		return;
	}

	Node temp = *first;

	*first = *second;
	*second = temp;
}
