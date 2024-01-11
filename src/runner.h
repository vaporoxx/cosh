#pragma once

#include "node.h"
#include <gmp.h>

int resolve(Node *node, mpq_t result, Node **failed, char **message);

int run(Node *node, Node **failed, char **message);
