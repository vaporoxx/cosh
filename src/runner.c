#include "runner.h"
#include <stdio.h>
#include <string.h>

static int resolve_function(Node *node, mpq_t result, Node **failed, char **message) {
	mpq_t value;
	mpq_init(value);

	if (resolve(node->left, value, failed, message)) {
		mpq_clear(value);
		return 1;
	}

	if (!strcmp(node->value, "abs")) {
		mpq_abs(result, value);

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

	if (!strcmp(node->value, "sgn")) {
		mpq_set_si(result, mpq_sgn(value), 1);

		mpq_clear(value);
		return 0;
	}

	*failed = node;
	*message = "unknown function";

	mpq_clear(value);
	return 1;
}

static int resolve_integer(Node *node, mpq_t result, Node **failed, char **message) {
	if (mpq_set_str(result, node->value, 10)) {
		*failed = node;
		*message = "invalid integer";

		return 1;
	}

	return 0;
}

static int resolve_operator(Node *node, mpq_t result, Node **failed, char **message) {
	mpq_t left;
	mpq_init(left);

	if (resolve(node->left, left, failed, message)) {
		mpq_clear(left);
		return 1;
	}

	mpq_t right;
	mpq_init(right);

	if (resolve(node->right, right, failed, message)) {
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

static int resolve_variable(Node *node, mpq_t result, Node **failed, char **message) {
	if (!strcmp(node->value, "x")) {
		mpq_set_ui(result, 5, 1);
		return 0;
	}

	*failed = node;
	*message = "unknown variable";

	return 1;
}

int resolve(Node *node, mpq_t result, Node **failed, char **message) {
	switch (node->type) {
		case NT_FUNCTION:
			return resolve_function(node, result, failed, message);
		case NT_INTEGER:
			return resolve_integer(node, result, failed, message);
		case NT_OPERATOR:
			return resolve_operator(node, result, failed, message);
		case NT_VARIABLE:
			return resolve_variable(node, result, failed, message);
	}

	*failed = node;
	*message = "unknown node type";

	return 1;
}

int run(Node *node, Node **failed, char **message) {
	if (!node) {
		return 0;
	}

	mpq_t value;
	mpq_init(value);

	if (resolve(node, value, failed, message)) {
		mpq_clear(value);
		return 1;
	}

	mpq_out_str(stdout, 10, value);
	putchar('\n');

	mpq_clear(value);
	return 0;
}