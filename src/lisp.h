#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>

enum TYPE;
struct any;
struct any_pair;

#define MAKE_DEF(TYP, ...) struct any* TYP ## _make(__VA_ARGS__)
MAKE_DEF(NIL);
MAKE_DEF(INTEGER, const int64_t value);
MAKE_DEF(PAIR, struct any* car, struct any* cdr);
MAKE_DEF(SYMBOL, const char *value);

#define PRINT_DEF(TYP) void TYP ## _print(const struct any* elem)
PRINT_DEF(NIL);
PRINT_DEF(INTEGER);
PRINT_DEF(PAIR);
PRINT_DEF(SYMBOL);

void print(const struct any* value);

struct any *car(const struct any *value);
struct any *cdr(const struct any *value);
struct any *cons(struct any *car, struct any *cdr);
struct any *reverse(struct any* to_reverse);

struct any* resolve(struct any* value);
struct any* eval(struct any* value);

#endif
