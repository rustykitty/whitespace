#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/whitespace.h"
#include "../common/parse/parse.h"
#include "../common/runtime/runtime.h"
#include "../common/error.h"

#include <sys/stat.h>

static inline ssize_t get_file_size(FILE* file) {
    struct stat64 st;
    if (fstat64(fileno(file), &st) == -1) {
        perror("fstat");
        return -1;
    }
    return st.st_size;
}

int main (int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "you need to provide at least one filename argument (`-` is stdin)\n");
        return 1;
    }
    char** p = &argv[1];
    do {
        FILE* file = NULL;
        if (strcmp(*p, "-") == 0) {
            file = stdin;
        } else {
            file = fopen(*p, "r");
            if (file == NULL) {
                perror(*p);
                return 1;
            }
        }
        ssize_t fileSize = get_file_size(file);
        if (fileSize == -1) {
            fclose(file);
            return 1;
        }
        char* buf = calloc(fileSize + 1, sizeof(char));
        fread(buf, 1, fileSize, file);
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
    } while (++(*p));
    return 0;
}