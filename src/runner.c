#include "runner.h"
#include <stdio.h>

static int print_node(Node *node, Node *root, size_t *failed) {
	if (!is_child(node, root)) {
		*failed = node->index;
		return 1;
	}

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

	if (node->left && print_node(node->left, root, failed)) {
		return 1;
	}

	if (node->right && print_node(node->right, root, failed)) {
		return 1;
	}

	return 0;
}

int run(Node *node, size_t *failed) {
	if (node) {
		return print_node(node, node, failed);
	}

	puts("(empty)");
	return 0;
}
