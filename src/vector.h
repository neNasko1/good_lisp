#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define VECTOR_DEF(TYPE)                                                \
	struct vector_##TYPE {                                              \
		uint64_t capacity;                                              \
		uint64_t size;                                                  \
		TYPE *elems;                                                    \
	};                                                                  \
	                                                                    \
	void vector_##TYPE ##_init(struct vector_##TYPE* vec) {             \
		vec->capacity = 1;                                              \
		vec->size = 0;                                                  \
		vec->elems = malloc(sizeof(TYPE));                              \
	}                                                                   \
	                                                                    \
	void vector_##TYPE ##_append(                                       \
		struct vector_##TYPE* vec,                                      \
		const TYPE new_value                                            \
	) {                                                                 \
		if(vec->size == vec->capacity) {                                \
			vec->capacity *= 2;                                         \
			TYPE *new_cont = malloc(vec->capacity * sizeof(TYPE));      \
			memcpy(new_cont, vec->elems, vec->size * sizeof(TYPE));     \
			vec->elems = new_cont;                                      \
		}                                                               \
		vec->elems[vec->size ++] = new_value;                           \
	}

VECTOR_DEF(char);

#endif
