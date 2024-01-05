#pragma once

#include <stddef.h>

void error(char *location, char *input, size_t failed);

char *get_input(void);
