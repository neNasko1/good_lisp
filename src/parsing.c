#ifndef PARSING_H
#define PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "parsing.h"
#include "vector.h"
#include "lisp.h"

struct file_state {
	FILE *fd;
	char curr;
};

struct file_state* file_state_init(FILE *fd) {
	struct file_state *ret = malloc(sizeof(struct file_state));
	ret->fd = fd;
	ret->curr = fgetc(fd);
	return ret;
}

void file_state_move(struct file_state* fs) {
	fs->curr = fgetc(fs->fd);
}

bool is_whitespace(const char c) {
	return c == ' ' || c == '\n';
}

void skip_whitespace(struct file_state* fs) {
	while(is_whitespace(fs->curr)) {
		file_state_move(fs);
	}
}

bool match_character(struct file_state* fs, const char c) {
	if(fs->curr != c) {
		return false;
	}
	file_state_move(fs);
	return true;
}

struct any* parse_ast(struct file_state* fs) {
	#define MATCH_OR_ERROR(chr)                                    \
		if(!match_character(fs, chr)) {                            \
			printf("Error in parsing for character %c", fs->curr); \
			exit(-1);                                              \
		}                                                          \

	skip_whitespace(fs);
	if(match_character(fs, EOF)) {
		return NULL;
	}

	if(fs->curr != '(') {
		struct vector_char ret;
		vector_char_init(&ret);

		bool is_number = true;

		while(!is_whitespace(fs->curr) && fs->curr != ')') {
			if(fs->curr < '0' || fs->curr > '9') {
				is_number = false;
			}
			vector_char_append(&ret, fs->curr);
			file_state_move(fs);
		}
		vector_char_append(&ret, '\0');

		if(is_number) {
			return INTEGER_make(atoi(ret.elems));
		} else {
			return SYMBOL_make(ret.elems);
		}
	}

	MATCH_OR_ERROR('(');
	skip_whitespace(fs);

	struct any *ret = NIL_make();

	while(fs->curr != ')') {
		skip_whitespace(fs);
		ret = PAIR_make(parse_ast(fs), ret);
		skip_whitespace(fs);
	}

	skip_whitespace(fs);
	MATCH_OR_ERROR(')');

	return reverse(ret);
}

void execute(FILE *fd) {
	struct file_state *fs = file_state_init(fd);

	struct vector_symbol_entry* ctx = create_vector_symbol_entry_default();

	while(true) {
		struct any* now = parse_ast(fs);

		if(now == NULL) {
			break;
		}

		eval(now, ctx);
	}
}

#endif
