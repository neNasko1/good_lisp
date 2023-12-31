#ifndef PARSING_H
#define PARSING_H

#include "parsing.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lisp.h"
#include "vector.h"

struct file_state {
    FILE *fd;
    char curr;
};

struct file_state *file_state_init(FILE *fd) {
    struct file_state *ret = malloc(sizeof(struct file_state));
    ret->fd = fd;
    ret->curr = fgetc(fd);
    return ret;
}

void file_state_move(struct file_state *fs) { fs->curr = fgetc(fs->fd); }

bool is_whitespace(const char c) { return c == ' ' || c == '\n'; }

void skip_whitespace(struct file_state *fs) {
    while (is_whitespace(fs->curr)) {
        file_state_move(fs);
    }
}

bool match_character(struct file_state *fs, const char c) {
    if (fs->curr != c) {
        return false;
    }
    file_state_move(fs);
    return true;
}

VECTOR_DEF(void *, vector_pointer);
VECTOR_IMPL(void *, vector_pointer);

struct any *parse_ast(struct file_state *fs, struct vector_pointer *free_list) {
#define MATCH_OR_ERROR(chr)                                    \
    if (!match_character(fs, chr)) {                           \
        printf("Error in parsing for character %c", fs->curr); \
        exit(-1);                                              \
    }

    skip_whitespace(fs);
    if (match_character(fs, EOF)) {
        return NULL;
    }

    if (fs->curr != '(') {
        struct string ret;
        string_init(&ret);

        bool is_number = true;

        while (!is_whitespace(fs->curr) && fs->curr != ')') {
            if (fs->curr < '0' || fs->curr > '9') {
                is_number = false;
            }
            string_append(&ret, fs->curr);
            file_state_move(fs);
        }
        string_append(&ret, '\0');

        vector_pointer_append(free_list, ret.elems);

        if (is_number) {
            return INTEGER_make(atoi(ret.elems));
        } else {
            return SYMBOL_make(ret.elems);
        }
    }

    MATCH_OR_ERROR('(');
    skip_whitespace(fs);

    struct any *ret = NIL_make();

    while (fs->curr != ')') {
        skip_whitespace(fs);
        ret = PAIR_make(parse_ast(fs, free_list), ret);
        skip_whitespace(fs);
    }

    skip_whitespace(fs);
    MATCH_OR_ERROR(')');

    return reverse(ret);
}

void execute(FILE *fd) {
    struct file_state *fs = file_state_init(fd);

    struct vector_symbol_entry *ctx = create_vector_symbol_entry_default();

    struct vector_pointer *free_list = malloc(sizeof(struct vector_pointer));
    vector_pointer_init(free_list);

    while (true) {
        struct any *now = parse_ast(fs, free_list);

        if (now == NULL) {
            break;
        }

        eval(now, ctx);
    }

    vector_symbol_entry_deinit(ctx);

    for (size_t i = 0; i < free_list->size; i++) {
        free(free_list->elems[i]);
    }
    vector_pointer_deinit(free_list);
}

#endif
