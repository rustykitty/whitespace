#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "whitespace.h"
#include "parse.h"
#include "runtime.h"
#include "ws_error.h"

#define INITIAL_BUFFER_SIZE 64

int main (int argc, char* argv[]) {
    char* filename;
    if (argc == 1) {
        filename = "-";
    } else {
        if (strcmp(argv[1], "--help") == 0) {
            printf("Usage: whitespace [filename]\n");
            return 0;
        }
        filename = argv[1];
    }
    FILE* file = NULL;
    if (strcmp(filename, "-") == 0) {
        file = stdin;
    } else {
        file = fopen(filename, "r");
        if (file == NULL) {
            perror(filename);
            return 1;
        }
    }
    char* buf;
    {
        size_t bufcap = INITIAL_BUFFER_SIZE;
        size_t bufsize = 0;
        buf = (char*) calloc(bufcap + 1, sizeof(char));
        if (!buf) {
            perror("calloc");
            return 1;
        }
        int c;
        while ((c = fgetc(file)) != EOF) {
            buf[bufsize] = c;
            ++bufsize;
            if (bufsize == bufcap) {
                char* tmp = realloc(buf, bufcap * 2 + 1);
                if (!tmp) {
                    perror("realloc");
                    return 1;
                }
                bufcap *= 2;
                buf = tmp;
            }
        }
    }

    if (file != stdin)
        fclose(file);

    size_t size;
    struct WS_statement* program = WS_parse(buf, &size);

    if (program == NULL) {
        goto error;
    }

    if (!WS_execute(program, size)) {
        goto error;
    }

    error:
    Err_perror();
    free(buf);
    return 1;
    return 0;
}