#ifndef BUILTIN_H
#define BUILTIN_H

#include <assert.h>

#include "lisp.h"

#define BUILTIN(FUNC)                              \
    struct any *builtin_##FUNC(struct any *params, \
                               struct vector_symbol_entry *ctx)

#define ASSERT_LEN(PARAMS_COUNT) assert(length(params) == PARAMS_COUNT);

#define RESOLVE_PARAMS() params = resolve(params, ctx);

BUILTIN(car) {
    ASSERT_LEN(1);
    RESOLVE_PARAMS();
    return car(car(params));
}

BUILTIN(cdr) {
    ASSERT_LEN(1);
    RESOLVE_PARAMS();
    return cdr(car(params));
}

BUILTIN(length) {
    ASSERT_LEN(1);
    RESOLVE_PARAMS();
    return INTEGER_make(length(car(params)));
}

BUILTIN(cons) {
    ASSERT_LEN(2);
    RESOLVE_PARAMS();
    return cons(car(params), car(cdr(params)));
}

BUILTIN(reverse) {
    ASSERT_LEN(1);
    RESOLVE_PARAMS();
    return reverse(car(params));
}

BUILTIN(print) {
    RESOLVE_PARAMS();

    printf(">>>");
    while (params->type != NIL) {
        printf(" ");
        print(car(params));
        params = cdr(params);
    }
    printf("\n");

    return NIL_make();
}

BUILTIN(sum) {
    RESOLVE_PARAMS();

    uint64_t ret = 0;
    while (params->type != NIL) {
        struct any *car_ = car(params);
        ASSERT_TYPE(car_, INTEGER);
        ret += car_->data.INTEGER_value;
        params = cdr(params);
    }

    return INTEGER_make(ret);
}

BUILTIN(eval) {
    ASSERT_LEN(1);

    struct any *value = car(params);

    switch (value->type) {
        case SYMBOL:
            return vector_symbol_entry_get(ctx, value->data.SYMBOL_value);
        case NIL:
        case INTEGER:
        case BUILTIN_FUNCTION:
            return value;
        case PAIR:
            break;
    }

    struct any *func = builtin_eval(cons(car(value), NIL_make()), ctx);
    struct any *func_params = cdr(value);

    if (func->type != BUILTIN_FUNCTION) {
        printf("Error trying to evaluate with something else than functions\n");
        printf("Function evaluated to: ");
        print(func);
        printf("\n");
        exit(-1);
    }

    ASSERT_TYPE(func, BUILTIN_FUNCTION);

    return func->data.BUILTIN_FUNCTION_value(func_params, ctx);
}

BUILTIN(quote) {
    ASSERT_LEN(1);

    return car(params);
}

BUILTIN(set) {
    ASSERT_LEN(2);

    struct any *name = car(params);
    struct any *value = builtin_eval(cdr(params), ctx);

    ASSERT_TYPE(name, SYMBOL);
    vector_symbol_entry_set(ctx, name->data.SYMBOL_value, value);

    return NIL_make();
}

#endif
