#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

void *xmalloc(size_t size) {
	void *pointer = malloc(size);

	if (!pointer) {
		fputs("cosh: could not allocate memory\n", stderr);
		abort();
	}

	return pointer;
}

void *xrealloc(void *pointer, size_t size) {
	pointer = realloc(pointer, size);

	if (!pointer) {
		fputs("cosh: could not allocate memory\n", stderr);
		abort();
	}

	return pointer;
}
