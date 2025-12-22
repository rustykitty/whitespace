#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#warning "Windows not officially supported"
#endif

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "whitespace.h"
#include "parse.h"
#include "runtime.h"
#include "ws_error.h"

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

    char* buf = NULL;
    size_t bufsize;

    if (strcmp(filename, "-") == 0) {

        // Create a temporary file
        FILE* tempfile = tmpfile();

        if (!tempfile) {
            perror("");
            return 1;
        }

        int c;
        while ((c = getchar()) != EOF) {
            char chr = c;
            fwrite(&chr, 1, 1, tempfile);
            if (ferror(tempfile)) {
                perror("");
                return 1;
            }
        }
        if (ferror(stdin)) {
            perror("");
            return 1;
        }
        bufsize = ftell(tempfile);
        int tempfd = fileno(tempfile);
        char* tempbuf = mmap(NULL, bufsize, PROT_READ, MAP_PRIVATE, tempfd, 0);
        if (!tempbuf) {
            perror("mmap");
            return 1;
        }
        buf = tempbuf;
        fclose(tempfile);
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
        buf = tempbuf;
        close(fd);
    }
    
    size_t size;
    struct WS_statement* program = WS_parse(buf, &size);

    if (program == NULL) {
        goto error;
    }

    if (!WS_execute(program, size)) {
        goto error;
    }


    munmap(buf, bufsize);

    return 0;

error:
    Err_perror();

    munmap(buf, bufsize);

    return 1;
}