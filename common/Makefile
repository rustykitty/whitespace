all: runtime/runtime.o error.o whitespace.o

runtime/runtime.o: runtime/runtime.c runtime/runtime.h whitespace.h
	$(CC) $(CFLAGS) -fPIC -c runtime/runtime.c -o runtime/runtime.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -fPIC -c error.c -o error.o

whitespace.o: whitespace.c whitespace.h
	$(CC) $(CFLAGS) -fPIC -c whitespace.c -o whitespace.o

clean:
	rm -rf *.o *.gch *.dSYM