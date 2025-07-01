all: whitespace_parser

whitespace_parser: whitespace_parser.c ../c
	$(CC) $(CFLAGS) -o whitespace_parser whitespace_parser.c \
	../c/runtime/runtime.o ../c/error.o ../c/whitespace.o ../c/parse/parse.o