parse.a: parse.o
	$(MAKE) -C ../error error.o
	ar rcs parse.a parse.o ../error/error.o

parse.o: parse.c
	$(CC) $(CFLAGS) -c parse.c -o parse.o
