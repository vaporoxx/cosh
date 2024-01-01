#pragma once

#include <stddef.h>

enum NodeType {
	NT_FUNCTION,
	NT_INTEGER,
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

int append_node(Node *node, Node *parent);

void free_node(Node *node);

int is_child(Node *node, Node *parent);

int is_expression(Node *node);

Node *last_operator(Node *node);

int new_node(Node **node, size_t index, NodeType type, char *value);

char *node_type(NodeType type);

int precedence(Node *node);
