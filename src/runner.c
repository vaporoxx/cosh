#include "runner.h"
#include <stdio.h>
#include <string.h>

static int resolve_function(Node *node, mpz_t result, Node **failed, char **message) {
	mpz_t value;
	mpz_init(value);

	if (resolve(node->left, value, failed, message)) {
		mpz_clear(value);
		return 1;
	}

	if (!strcmp(node->value, "abs")) {
		mpz_abs(result, value);

		mpz_clear(value);
		return 0;
	}

	*failed = node;
	*message = "unknown function";

	mpz_clear(value);
	return 1;
}

static int resolve_integer(Node *node, mpz_t result, Node **failed, char **message) {
	if (mpz_set_str(result, node->value, 10)) {
		*failed = node;
		*message = "invalid integer";

		return 1;
	}

	return 0;
}

static int resolve_operator(Node *node, mpz_t result, Node **failed, char **message) {
	char value = node->value[0];

	if (!strchr("%*+-/^", value)) {
		*failed = node;
		*message = "unknown operator";

		return 1;
	}

	mpz_t right;
	mpz_init(right);

	if (resolve(node->right, right, failed, message)) {
		mpz_clear(right);
		return 1;
	}

	if ((value == '%' || value == '/') && !mpz_sgn(right)) {
		*failed = node;
		*message = "division by zero";

		mpz_clear(right);
		return 1;
	}

	if (value == '^' && !mpz_fits_ulong_p(right)) {
		*failed = node;
		*message = "invalid exponent";

		mpz_clear(right);
		return 1;
	}

	mpz_t left;
	mpz_init(left);

	if (resolve(node->left, left, failed, message)) {
		mpz_clear(left);
		mpz_clear(right);

		return 1;
	}

	if (value == '%') {
		mpz_mod(result, left, right);
	} else if (value == '*') {
		mpz_mul(result, left, right);
	} else if (value == '+') {
		mpz_add(result, left, right);
	} else if (value == '-') {
		mpz_sub(result, left, right);
	} else if (value == '/') {
		mpz_div(result, left, right);
	} else if (value == '^') {
		mpz_pow_ui(result, left, mpz_get_ui(right));
	}

	mpz_clear(left);
	mpz_clear(right);

	return 0;
}

static int resolve_variable(Node *node, mpz_t result, Node **failed, char **message) {
	if (!strcmp(node->value, "x")) {
		mpz_set_ui(result, 5);
		return 0;
	}

	*failed = node;
	*message = "unknown variable";

	return 1;
}

int resolve(Node *node, mpz_t result, Node **failed, char **message) {
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

	mpz_t value;
	mpz_init(value);

	if (resolve(node, value, failed, message)) {
		return 1;
	}

	mpz_out_str(stdout, 10, value);
	putchar('\n');

	mpz_clear(value);
	return 0;
}
