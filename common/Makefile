all: exec_tree.o error.o whitespace.o

exec_tree.o: exec_tree.c exec_tree.h whitespace.h
	$(CC) $(CFLAGS) -fPIC -c exec_tree.c -o exec_tree.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -fPIC -c error.c -o error.o

whitespace.o: whitespace.c whitespace.h
	$(CC) $(CFLAGS) -fPIC -c whitespace.c -o whitespace.o

whitespace_parser: whitespace_parser.c whitespace.o exec_tree.o error.o
	$(CC) $(CFLAGS) -o out/whitespace_parser whitespace_parser.c exec_tree.o error.o whitespace.o

clean:
	rm -rf *.o *.gch *.dSYM