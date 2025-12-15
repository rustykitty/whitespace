#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#warning "Windows not officially supported"
#endif

#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

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

    int is_stdin = 0;

    char* buf = NULL;
    size_t bufsize;

    if (strcmp(filename, "-") == 0) {
        is_stdin = 1;

        // Read all of stdin into memory

        FILE* temp = open_memstream(&buf, &bufsize);

        int c;
        while ((c = getchar()) != EOF) {
            fputc(c, temp);
        }
        if (ferror(stdin)) {
            perror("");
            return 1;
        }
        fclose(temp);
    } else {
        // Map the filename into memory

        int fd = open(filename, O_RDONLY);
        if (fd < 0) {
            perror(filename);
            return 1;
        }
        struct stat filestat;
        fstat(fd, &filestat);
        bufsize = filestat.st_size;
        char* tempbuf = mmap(NULL, bufsize, PROT_READ, MAP_PRIVATE, fd, 0);
        if (!tempbuf) {
            perror("mmap");
            return 1;
        }
    }
    
    size_t size;
    struct WS_statement* program = WS_parse(buf, &size);

    if (program == NULL) {
        goto error;
    }

    if (!WS_execute(program, size)) {
        goto error;
    }

    if (is_stdin) {
        free(buf);
    } else {
        munmap(buf, bufsize);
    }

    return 0;

error:
    Err_perror();
    free(buf);

    if (is_stdin) {
        free(buf);
    } else {
        munmap(buf, bufsize);
    }

    return 1;
}