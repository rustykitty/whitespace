MAKEFILE_DIR = $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

PROJECT_ROOT = $(MAKEFILE_DIR)

CFLAGS += -Wall -Wextra -std=gnu99

build: runtime.o parse.o ws_error.o whitespace_parser

whitespace_parser: whitespace_parser.c parse.o runtime.o ws_error.o whitespace.h
	$(CC) $(CFLAGS) -o whitespace_parser whitespace_parser.c ./parse.o ./runtime.o ws_error.o $(LDLIBS)

parse.o: parse.c parse.h ws_error.o whitespace.h
	$(CC) $(CFLAGS) -c -o parse.o parse.c

ws_error.o: ws_error.c ws_error.h
	$(CC) $(CFLAGS) -c -o ws_error.o ws_error.c

runtime.o: runtime.h runtime.c whitespace.h
	$(CC) $(CFLAGS) -c -o runtime.o runtime.c

.PHONY: clean
clean:
	rm -f whitespace_parser $(wildcard *.o)

SUBDIRS = runtime

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
