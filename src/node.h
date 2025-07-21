#pragma once

#include <stdio.h>

enum NodeType {
	NT_ASSIGNMENT,
	NT_FUNCTION,
	NT_LITERAL,
	NT_OPERATOR,
	NT_VARIABLE,
};

typedef enum NodeType NodeType;

struct Node {
	int grouped;
	size_t index;
	struct Node *left;
	struct Node *right;
	NodeType type;
	char *value;
};

typedef struct Node Node;

void free_node(Node *node);

int is_child(Node *node, Node *parent);

int is_expression(Node *node);

Node *new_node(size_t index, NodeType type, char *value);

char *node_type(NodeType type);

int precedence(Node *node);

void swap_nodes(Node *first, Node *second);
