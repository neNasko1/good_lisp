#ifndef LISP_H
#define LISP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

struct vector_symbol_entry;
struct vector_any;

struct gc_header {
    uint64_t last_seen;
};

enum TYPE { NIL, INTEGER, PAIR, SYMBOL, BUILTIN_FUNCTION };

struct any_pair {
    struct any *car;
    struct any *cdr;
};

struct any {
    struct gc_header mem_info;
    enum TYPE type;
    union {
        int64_t INTEGER_value;
        struct any_pair PAIR_value;
        const char *SYMBOL_value;
        struct any *(*BUILTIN_FUNCTION_value)(struct any *,
                                              struct vector_symbol_entry *);
    } data;
};

#define MAKE_DEF(TYP, ...) struct any *TYP##_make(__VA_ARGS__)
MAKE_DEF(NIL);
MAKE_DEF(INTEGER, const int64_t value);
MAKE_DEF(PAIR, struct any *car, struct any *cdr);
MAKE_DEF(SYMBOL, const char *value);
MAKE_DEF(BUILTIN_FUNCTION,
         struct any *(*func)(struct any *, struct vector_symbol_entry *));

#define PRINT_DEF(TYP) void TYP##_print(struct any *elem)
PRINT_DEF(NIL);
PRINT_DEF(INTEGER);
PRINT_DEF(PAIR);
PRINT_DEF(SYMBOL);
PRINT_DEF(BUILTIN_FUNCTION);
void print(struct any *value);

#define ASSERT_TYPE(value, TYPE)                      \
    if (value->type != TYPE) {                        \
        printf("\nReal value is %d \n", value->type); \
        print(value);                                 \
        assert(value->type == TYPE);                  \
    }

struct any *car(struct any *value);
struct any *cdr(struct any *value);
struct any *cons(struct any *car, struct any *cdr);
struct any *reverse(struct any *to_reverse);
uint64_t length(struct any *list);

typedef struct {
    const char *name;
    struct any *value;
} symbol_entry;

VECTOR_DEF(symbol_entry, vector_symbol_entry);

struct vector_symbol_entry *create_vector_symbol_entry_default();

void vector_symbol_entry_set(struct vector_symbol_entry *table,
                             const char *name, struct any *value);

struct any *vector_symbol_entry_get(struct vector_symbol_entry *table,
                                    const char *name);

struct any *resolve(struct any *value, struct vector_symbol_entry *ctx);
struct any *eval(struct any *value, struct vector_symbol_entry *ctx);

#endif
