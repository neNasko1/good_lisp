#ifndef BUILTIN_H
#define BUILTIN_H

#include <assert.h>

#include "lisp.h"

#define BUILTIN(FUNC) \
	struct any* builtin_##FUNC(struct any *params)

#define ASSERT_LEN(PARAMS_COUNT) \
	assert(length(params) == PARAMS_COUNT);

BUILTIN(car) {
	ASSERT_LEN(1);
	return car(car(params));
}

BUILTIN(cdr) {
	ASSERT_LEN(1);
	return cdr(car(params));
}

BUILTIN(length) {
	ASSERT_LEN(1);
	return INTEGER_make(length(car(params)));
}

BUILTIN(cons) {
	ASSERT_LEN(2);
	return cons(
		car(params),
		car(cdr(params))
	);
}

BUILTIN(reverse) {
	ASSERT_LEN(1);
	return reverse(car(params));
}

BUILTIN(print) {
	printf(">>>");
	while(params->type != NIL) {
		printf(" ");
		printf("[");
		print(car(params));
		printf("]");
		params = cdr(params);
	}
	printf("\n");
	return NIL_make();
}


#endif
