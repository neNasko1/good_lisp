#include "lisp.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"

MAKE_DEF(NIL) {
    struct any *ret = malloc(sizeof(struct any));
    ret->type = NIL;
    return ret;
}

MAKE_DEF(INTEGER, const int64_t value) {
    struct any *ret = NIL_make();
    ret->type = INTEGER;
    ret->data.INTEGER_value = value;
    return ret;
}

MAKE_DEF(PAIR, struct any *car, struct any *cdr) {
    struct any *ret = NIL_make();
    ret->type = PAIR;
    ret->data.PAIR_value.car = car;
    ret->data.PAIR_value.cdr = cdr;
    return ret;
}

MAKE_DEF(SYMBOL, const char *value) {
    struct any *ret = NIL_make();
    ret->type = SYMBOL;
    ret->data.SYMBOL_value = value;
    return ret;
}

MAKE_DEF(BUILTIN_FUNCTION,
         struct any *(*func)(struct any *, struct vector_symbol_entry *)) {
    struct any *ret = NIL_make();
    ret->type = BUILTIN_FUNCTION;
    ret->data.BUILTIN_FUNCTION_value = func;
    return ret;
}

PRINT_DEF(NIL) { printf("()"); }

PRINT_DEF(INTEGER) { printf("%lld", elem->data.INTEGER_value); }

bool is_list(struct any *value) {
    if (value->type == NIL) {
        return true;
    }
    if (value->type == PAIR) {
        return is_list(cdr(value));
    }
    return false;
}

PRINT_DEF(PAIR) {
    if (is_list(elem)) {
        printf("(");
        print(car(elem));

        struct any *curr = cdr(elem);
        while (curr->type != NIL) {
            printf(" ");
            print(car(curr));
            curr = cdr(curr);
        }
        printf(")");
    } else {
        printf("(");
        print(car(elem));
        printf(", ");
        print(cdr(elem));
        printf(")");
    }
}

PRINT_DEF(SYMBOL) { printf("%s", elem->data.SYMBOL_value); }

PRINT_DEF(BUILTIN_FUNCTION) { printf("<builtin-func>"); }

struct any *car(struct any *value) {
    ASSERT_TYPE(value, PAIR);
    return value->data.PAIR_value.car;
}

struct any *cdr(struct any *value) {
    ASSERT_TYPE(value, PAIR);
    return value->data.PAIR_value.cdr;
}

struct any *cons(struct any *car, struct any *cdr) {
    return PAIR_make(car, cdr);
}

uint64_t length(struct any *list) {
    uint64_t len = 0;

    while (list->type != NIL) {
        len++;
        list = cdr(list);
    }

    return len;
}

struct any *reverse(struct any *to_reverse) {
    struct any *ret = NIL_make();

    while (to_reverse->type != NIL) {
        struct any *car_ = car(to_reverse);
        struct any *cdr_ = cdr(to_reverse);
        ret = cons(car_, ret);
        to_reverse = cdr_;
    }

    return ret;
}

void print(struct any *value) {
#define SWITCH_CASE(TYPE)                                                      \
    case TYPE:                                                                 \
        TYPE##_print(value);                                                   \
        break;

    switch (value->type) {
        SWITCH_CASE(NIL);
        SWITCH_CASE(INTEGER);
        SWITCH_CASE(PAIR);
        SWITCH_CASE(SYMBOL);
        SWITCH_CASE(BUILTIN_FUNCTION);
    }

    fflush(stdout);
}

struct any *resolve(struct any *value, struct vector_symbol_entry *ctx) {
    switch (value->type) {
    case PAIR:
        return cons(eval(car(value), ctx), resolve(cdr(value), ctx));
    case SYMBOL:
        return vector_symbol_entry_get(ctx, value->data.SYMBOL_value);
    case NIL:
    case INTEGER:
    case BUILTIN_FUNCTION:
        return value;
    }
}

struct any *eval(struct any *value, struct vector_symbol_entry *ctx) {
    return builtin_eval(cons(value, NIL_make()), ctx);
}

struct vector_symbol_entry *create_vector_symbol_entry_default() {
    struct vector_symbol_entry *ret =
        malloc(sizeof(struct vector_symbol_entry));

    vector_symbol_entry_init(ret);

    vector_symbol_entry_set(ret, "print", BUILTIN_FUNCTION_make(builtin_print));
    vector_symbol_entry_set(ret, "+", BUILTIN_FUNCTION_make(builtin_sum));
    vector_symbol_entry_set(ret, "eval", BUILTIN_FUNCTION_make(builtin_eval));
    vector_symbol_entry_set(ret, "quote", BUILTIN_FUNCTION_make(builtin_quote));

    return ret;
}

void vector_symbol_entry_set(struct vector_symbol_entry *table,
                             const char *name, struct any *value) {
    for (uint64_t i = 0; i < table->size; i++) {
        if (strcmp(name, table->elems[i].name) == 0) {
            table->elems[i].value = value;
            return;
        }
    }

    symbol_entry to_push;
    to_push.name = name;
    to_push.value = value;
    vector_symbol_entry_append(table, to_push);
}

struct any *vector_symbol_entry_get(struct vector_symbol_entry *table,
                                    const char *name) {
    for (uint64_t i = 0; i < table->size; i++) {
        if (strcmp(name, table->elems[i].name) == 0) {
            return table->elems[i].value;
        }
    }

    return NIL_make();
}
