all: runtime/runtime.o error.o whitespace.o heap.o

runtime/runtime.o: runtime/runtime.c runtime/runtime.h whitespace.h heap.h
	$(CC) $(CFLAGS) -fPIC -c runtime/runtime.c -o runtime/runtime.o

runtime/heap.o: runtime/heap.c runtime/heap.h
	$(CC) $(CFLAGS) -fPIC -c runtime/heap.c -o runtime/heap.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -fPIC -c error.c -o error.o

whitespace.o: whitespace.c whitespace.h
	$(CC) $(CFLAGS) -fPIC -c whitespace.c -o whitespace.o

clean:
	rm -rf *.o *.gch *.dSYM