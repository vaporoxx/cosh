#pragma once

#include <gmp.h>

struct Var {
	char *name;
	mpq_t value;
};

typedef struct Var Var;

struct Vars {
	Var *elements;
	size_t size;
};

typedef struct Vars Vars;

void free_vars(Vars *vars);

Var *get_var(Vars *vars, char *name);

Vars new_vars(void);

void set_var(Vars *vars, char *name, mpq_t value);
