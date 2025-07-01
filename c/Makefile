makefile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_path := $(patsubst %/,%,$(dir $(makefile_path)))
project_root := $(patsubst %/,%,$(dir $(current_path)))

all: whitespace_parser

whitespace_parser: whitespace_parser.c ../common
	$(CC) $(CFLAGS) -o whitespace_parser whitespace_parser.c \
	$(wildcard $(project_root)/common/*.o)