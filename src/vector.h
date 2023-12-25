#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_DEF(TYPE)                                    \
    struct vector_##TYPE {                                  \
        uint64_t capacity;                                  \
        uint64_t size;                                      \
        TYPE *elems;                                        \
    };                                                      \
    void vector_##TYPE##_init(struct vector_##TYPE *vec);   \
    void vector_##TYPE##_deinit(struct vector_##TYPE *vec); \
    void vector_##TYPE##_append(struct vector_##TYPE *vec,  \
                                const TYPE new_value);

#define VECTOR_IMPL(TYPE)                                                   \
    void vector_##TYPE##_init(struct vector_##TYPE *vec) {                  \
        vec->capacity = 1;                                                  \
        vec->size = 0;                                                      \
        vec->elems = malloc(sizeof(TYPE));                                  \
    }                                                                       \
                                                                            \
    void vector_##TYPE##_deinit(struct vector_##TYPE *vec) {                \
        free(vec->elems);                                                   \
    }                                                                       \
                                                                            \
    void vector_##TYPE##_append(struct vector_##TYPE *vec,                  \
                                const TYPE new_value) {                     \
        if (vec->size == vec->capacity) {                                   \
            vec->capacity *= 2;                                             \
            vec->elems = realloc(vec->elems, vec->capacity * sizeof(TYPE)); \
        }                                                                   \
        vec->elems[vec->size++] = new_value;                                \
    }

VECTOR_DEF(char);

#endif
