#include <stdio.h>
#include <stdlib.h>

#include "lisp.h"
#include "parsing.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("No input files");
        return -1;
    } else if (argc > 2) {
        printf("Too many arguments");
        return -2;
    }

    const char *input_file = argv[1];
    FILE *fd = fopen(input_file, "r");
    execute(fd);

    return 0;
}
