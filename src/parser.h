#pragma once

#include "node.h"
#include "token.h"

int parse(Tokens *tokens, Node **node, size_t *failed);
