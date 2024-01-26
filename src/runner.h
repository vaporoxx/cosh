#pragma once

#include "node.h"
#include "var.h"

int resolve(Node *node, mpq_t result, Vars *vars, Node **failed, char **message);

int run(Node *node, Vars *vars, Node **failed, char **message);
