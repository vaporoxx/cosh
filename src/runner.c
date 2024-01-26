#include "runner.h"
#include <stdio.h>
#include <string.h>

static int resolve_assignment(Node *node, mpq_t result, Vars *vars, Node **failed, char **message) {
	mpq_t value;
	mpq_init(value);

	if (resolve(node->right, value, vars, failed, message)) {
		mpq_clear(value);
		return 1;
	}

	mpq_set(result, value);
	set_var(vars, node->left->value, value);

	mpq_clear(value);
	return 0;
}

static int resolve_function(Node *node, mpq_t result, Vars *vars, Node **failed, char **message) {
	mpq_t value;
	mpq_init(value);

	if (resolve(node->left, value, vars, failed, message)) {
		mpq_clear(value);
		return 1;
	}

	if (!strcmp(node->value, "abs")) {
		mpq_abs(result, value);

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "ceil")) {
		mpz_cdiv_q(mpq_numref(result), mpq_numref(value), mpq_denref(value));

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "den")) {
		mpq_set_z(result, mpq_denref(value));

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "floor")) {
		mpz_fdiv_q(mpq_numref(result), mpq_numref(value), mpq_denref(value));

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "inv")) {
		if (!mpq_sgn(value)) {
			*failed = node;
			*message = "argument must be non-zero";

			mpq_clear(value);
			return 1;
		}

		mpq_inv(result, value);

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "neg")) {
		mpq_neg(result, value);

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "num")) {
		mpq_set_z(result, mpq_numref(value));

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "sgn")) {
		mpq_set_si(result, mpq_sgn(value), 1);

		mpq_clear(value);
		return 0;
	}

	if (!strcmp(node->value, "trunc")) {
		mpz_tdiv_q(mpq_numref(result), mpq_numref(value), mpq_denref(value));

		mpq_clear(value);
		return 0;
	}

	*failed = node;
	*message = "unknown function";

	mpq_clear(value);
	return 1;
}

static int resolve_literal(Node *node, mpq_t result, Node **failed, char **message) {
	char *current = strtok(node->value, ",.");

	mpq_t value;
	mpq_init(value);

	if (mpq_set_str(value, current, 10)) {
		mpq_clear(value);

		*failed = node;
		*message = "invalid integral part";

		return 1;
	}

	current = strtok(NULL, "");

	if (current) {
		mpq_t fraction;
		mpq_init(fraction);

		if (mpq_set_str(fraction, current, 10)) {
			mpq_clears(fraction, value, NULL);

			*failed = node;
			*message = "invalid fractional part";

			return 1;
		}

		mpz_ui_pow_ui(mpq_denref(fraction), 10, strlen(current));
		mpq_canonicalize(fraction);

		mpq_add(value, fraction, value);
		mpq_clear(fraction);
	}

	mpq_set(result, value);
	mpq_clear(value);

	return 0;
}

static int resolve_operator(Node *node, mpq_t result, Vars *vars, Node **failed, char **message) {
	mpq_t left;
	mpq_init(left);

	if (resolve(node->left, left, vars, failed, message)) {
		mpq_clear(left);
		return 1;
	}

	mpq_t right;
	mpq_init(right);

	if (resolve(node->right, right, vars, failed, message)) {
		mpq_clears(left, right, NULL);
		return 1;
	}

	char value = node->value[0];

	if (value == '%') {
		if (mpz_cmp_ui(mpq_denref(left), 1) || mpz_cmp_ui(mpq_denref(right), 1)) {
			*failed = node;
			*message = "operands must be integers";

			mpq_clears(left, right, NULL);
			return 1;
		}

		if (!mpq_sgn(right)) {
			*failed = node;
			*message = "division by zero";

			mpq_clears(left, right, NULL);
			return 1;
		}

		mpz_tdiv_r(mpq_numref(result), mpq_numref(left), mpq_numref(right));

		mpq_clears(left, right, NULL);
		return 0;
	}

	if (value == '*') {
		mpq_mul(result, left, right);

		mpq_clears(left, right, NULL);
		return 0;
	}

	if (value == '+') {
		mpq_add(result, left, right);

		mpq_clears(left, right, NULL);
		return 0;
	}

	if (value == '-') {
		mpq_sub(result, left, right);

		mpq_clears(left, right, NULL);
		return 0;
	}

	if (value == '/') {
		if (!mpq_sgn(right)) {
			*failed = node;
			*message = "division by zero";

			mpq_clears(left, right, NULL);
			return 1;
		}

		mpq_div(result, left, right);

		mpq_clears(left, right, NULL);
		return 0;
	}

	if (value == '^') {
		if (mpz_cmp_ui(mpq_denref(right), 1)) {
			*failed = node;
			*message = "exponent must be an integer";

			mpq_clears(left, right, NULL);
			return 1;
		}

		if (!mpz_fits_sshort_p(mpq_numref(right))) {
			*failed = node;
			*message = "exponent out of range";

			mpq_clears(left, right, NULL);
			return 1;
		}

		if (!mpq_sgn(left) && mpq_sgn(right) == -1) {
			*failed = node;
			*message = "negative exponent requires non-zero base";

			mpq_clears(left, right, NULL);
			return 1;
		}

		mpz_pow_ui(mpq_numref(result), mpq_numref(left), mpz_get_ui(mpq_numref(right)));
		mpz_pow_ui(mpq_denref(result), mpq_denref(left), mpz_get_ui(mpq_numref(right)));

		if (mpq_sgn(right) == -1) {
			mpq_inv(result, result);
		}

		mpq_clears(left, right, NULL);
		return 0;
	}

	*failed = node;
	*message = "unknown operator";

	mpq_clears(left, right, NULL);
	return 1;
}

static int resolve_variable(Node *node, mpq_t result, Vars *vars, Node **failed, char **message) {
	Var *var = get_var(vars, node->value);

	if (var) {
		mpq_set(result, var->value);
		return 0;
	}

	*failed = node;
	*message = "unknown variable";

	return 1;
}

int resolve(Node *node, mpq_t result, Vars *vars, Node **failed, char **message) {
	switch (node->type) {
		case NT_ASSIGNMENT:
			return resolve_assignment(node, result, vars, failed, message);
		case NT_FUNCTION:
			return resolve_function(node, result, vars, failed, message);
		case NT_LITERAL:
			return resolve_literal(node, result, failed, message);
		case NT_OPERATOR:
			return resolve_operator(node, result, vars, failed, message);
		case NT_VARIABLE:
			return resolve_variable(node, result, vars, failed, message);
	}

	*failed = node;
	*message = "unknown node type";

	return 1;
}

int run(Node *node, Vars *vars, Node **failed, char **message) {
	if (!node) {
		return 0;
	}

	mpq_t value;
	mpq_init(value);

	if (resolve(node, value, vars, failed, message)) {
		mpq_clear(value);
		return 1;
	}

	mpq_out_str(stdout, 10, value);
	putchar('\n');

	mpq_clear(value);
	return 0;
}
