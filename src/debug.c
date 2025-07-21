#include "debug.h"

#include <stdio.h>

static void debug_node(Node *node, Node *root) {
	Node *current = root;

	while (current != node) {
		Node *next = NULL;

		if (is_child(node, current->left)) {
			next = current->left;
		} else {
			next = current->right;
		}

		if (next == current->left && current->right) {
			if (next == node) {
				fputs("├── ", stdout);
			} else {
				fputs("│   ", stdout);
			}
		} else {
			if (next == node) {
				fputs("└── ", stdout);
			} else {
				fputs("    ", stdout);
			}
		}

		current = next;
	}

	if (node->grouped) {
		printf("%s (%s) (column %zu) (grouped)\n", node_type(node->type), node->value, node->index + 1);
	} else {
		printf("%s (%s) (column %zu) \n", node_type(node->type), node->value, node->index + 1);
	}

	if (node->left) {
		debug_node(node->left, root);
	}

	if (node->right) {
		debug_node(node->right, root);
	}
}

void debug(Node *node) {
	if (node) {
		debug_node(node, node);
	}
}
