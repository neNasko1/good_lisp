#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_DEF(TYPE, CONT_NAME)                 \
    struct CONT_NAME {                              \
        uint64_t capacity;                          \
        uint64_t size;                              \
        TYPE *elems;                                \
    };                                              \
    void CONT_NAME##_init(struct CONT_NAME *vec);   \
    void CONT_NAME##_deinit(struct CONT_NAME *vec); \
    void CONT_NAME##_append(struct CONT_NAME *vec, const TYPE new_value);

#define VECTOR_IMPL(TYPE, CONT_NAME)                                        \
    void CONT_NAME##_init(struct CONT_NAME *vec) {                          \
        vec->capacity = 1;                                                  \
        vec->size = 0;                                                      \
        vec->elems = malloc(sizeof(TYPE));                                  \
    }                                                                       \
                                                                            \
    void CONT_NAME##_deinit(struct CONT_NAME *vec) { free(vec->elems); }    \
                                                                            \
    void CONT_NAME##_append(struct CONT_NAME *vec, const TYPE new_value) {  \
        if (vec->size == vec->capacity) {                                   \
            vec->capacity *= 2;                                             \
            vec->elems = realloc(vec->elems, vec->capacity * sizeof(TYPE)); \
        }                                                                   \
        vec->elems[vec->size++] = new_value;                                \
    }

VECTOR_DEF(char, string);

#endif
