#pragma once

#include <stddef.h>

void error(char *location, char *input, size_t failed);

int get_input(char **input);
