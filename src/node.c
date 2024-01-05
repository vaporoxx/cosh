#include "node.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>

int append_node(Node *node, Node *parent) {
	if (!node || !parent || parent->right) {
		return 1;
	}

	if (parent->left) {
		parent->right = node;
	} else {
		parent->left = node;
	}

	return 0;
}

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

	if (node->type == NT_INTEGER || node->type == NT_VARIABLE) {
		return 1;
	}

	if (node->type == NT_FUNCTION && is_expression(node->left)) {
		return 1;
	}

	if (node->type == NT_OPERATOR && is_expression(node->left) && is_expression(node->right)) {
		return 1;
	}

	return 0;
}

Node *last_operator(Node *node) {
	if (!node || node->type != NT_OPERATOR) {
		return NULL;
	}

	while (node->right && node->right->type == NT_OPERATOR) {
		node = node->right;
	}

	return node;
}

Node *new_node(size_t index, NodeType type, char *value) {
	Node *node = xmalloc(sizeof(Node));

	*node = (Node){0, index, NULL, NULL, type, value};
	return node;
}

char *node_type(NodeType type) {
	switch (type) {
		case NT_FUNCTION:
			return "Function";
		case NT_INTEGER:
			return "Integer";
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
