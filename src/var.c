#include "var.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>

void free_vars(Vars *vars) {
	for (size_t i = 0; i < vars->size; i += 1) {
		Var *var = vars->elements + i;

		free(var->name);
		mpq_clear(var->value);
	}

	free(vars->elements);
}

Var *get_var(Vars *vars, char *name) {
	for (size_t i = 0; i < vars->size; i += 1) {
		Var *var = vars->elements + i;

		if (!strcmp(var->name, name)) {
			return var;
		}
	}

	return NULL;
}

Vars new_vars(void) {
	return (Vars){NULL, 0};
}

void set_var(Vars *vars, char *name, mpq_t value) {
	Var *var = get_var(vars, name);

	if (!var) {
		vars->elements = xrealloc(vars->elements, sizeof(Var) * (vars->size + 1));
		vars->size += 1;

		var = vars->elements + vars->size - 1;
		var->name = xmalloc(strlen(name) + 1);

		strcpy(var->name, name);
		mpq_init(var->value);
	}

	mpq_set(var->value, value);
}
