#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "lisp.h"

enum TYPE {
	NIL,
	INTEGER,
	PAIR,
	SYMBOL
};

struct any_pair {
	struct any *car;
	struct any *cdr;
};

struct any {
	enum TYPE type;
	union {
		int64_t INTEGER_value;
		struct any_pair PAIR_value;
		const char *SYMBOL_value;
	} data;
};

MAKE_DEF(NIL) {
	struct any* ret = malloc(sizeof(struct any));
	ret->type = NIL;
	return ret;
}

MAKE_DEF(INTEGER, const int64_t value) {
	struct any* ret = NIL_make();
	ret->type = INTEGER;
	ret->data.INTEGER_value = value;
	return ret;
}

MAKE_DEF(PAIR, struct any* car, struct any* cdr) {
	struct any* ret = NIL_make();
	ret->type = PAIR;
	ret->data.PAIR_value.car = car;
	ret->data.PAIR_value.cdr = cdr;
	return ret;
}

MAKE_DEF(SYMBOL, const char *value) {
	struct any* ret = NIL_make();
	ret->type = SYMBOL;
	ret->data.SYMBOL_value = value;
	return ret;
}

PRINT_DEF(NIL) {
	printf("()");
}

PRINT_DEF(INTEGER) {
	printf("%lld", elem->data.INTEGER_value);
}

PRINT_DEF(PAIR) {
	printf("(");
	print(car(elem));
	printf(", ");
	print(cdr(elem));
	printf(")");
}

PRINT_DEF(SYMBOL) {
	printf("%s", elem->data.SYMBOL_value);
}

struct any *car(const struct any *value) {
	assert(value->type == PAIR);
	return value->data.PAIR_value.car;
}

struct any *cdr(const struct any *value) {
	assert(value->type == PAIR);
	return value->data.PAIR_value.cdr;
}

struct any *cons(struct any *car, struct any *cdr) {
	return PAIR_make(car, cdr);
}

struct any *reverse(struct any* to_reverse) {
	struct any *ret = NIL_make();

	while(to_reverse->type != NIL) {
		struct any* car_ = car(to_reverse);
		struct any* cdr_ = cdr(to_reverse);
		ret = cons(car_, ret);
		to_reverse = cdr_;
	}

	return ret;
}

void print(const struct any* value) {
	#define SWITCH_CASE(TYPE)      \
		case TYPE:                 \
			TYPE ## _print(value); \
			break;                 \

	switch(value->type) {
		SWITCH_CASE(NIL);
		SWITCH_CASE(INTEGER);
		SWITCH_CASE(PAIR);
		SWITCH_CASE(SYMBOL);
	}

	fflush(stdout);
}

struct any* resolve(struct any* value) {
	switch(value->type) {
		case NIL:
			return value;
		case INTEGER:
			return value;
		case PAIR:
			return cons(eval(car(value)), resolve(cdr(value)));
		case SYMBOL:
			return value;
	}
}

struct any* eval(struct any* value) {
	switch(value->type) {
		case NIL:
			return value;
		case INTEGER:
			return value;
		case SYMBOL:
			return value;
		case PAIR:
			break;
	}

	assert(value->type == PAIR);

	struct any* func = car(value);
	struct any* params = cdr(value);

	if(func->type != SYMBOL) {
		return value;
	}

	assert(func->type == SYMBOL);

	if(strcmp("print", func->data.SYMBOL_value) == 0) {
		params = resolve(params);
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

	return value;
}


