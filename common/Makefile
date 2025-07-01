all: exec_tree.o error.o parse_whitespace.o

exec_tree.o: exec_tree.c exec_tree.h whitespace.h
	$(CC) $(CFLAGS) -fPIC -c exec_tree.c -o exec_tree.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -fPIC -c error.c -o error.o

parse_whitespace.o: parse_whitespace.c parse_whitespace.h whitespace.h
	$(CC) $(CFLAGS) -fPIC -c parse_whitespace.c -o parse_whitespace.o

whitespace_parser: whitespace_parser.c parse_whitespace.o whitespace.h exec_tree.o error.o
	$(CC) $(CFLAGS) -o out/whitespace_parser whitespace_parser.c exec_tree.o error.o parse_whitespace.o

clean:
	rm -rf *.o *.gch *.dSYM