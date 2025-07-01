all: runtime/runtime.o error.o whitespace.o runtime/heap.o parse/parse.o

parse/parse.o: parse/parse.c parse/parse.h whitespace.h
	$(CC) $(CFLAGS) -fPIC -c parse/parse.c -o parse/parse.o

runtime/runtime.o: runtime/runtime.c runtime/runtime.h whitespace.h runtime/heap.h
	$(CC) $(CFLAGS) -fPIC -c runtime/runtime.c -o runtime/runtime.o

runtime/heap.o: runtime/heap.cpp runtime/heap.h
	$(CXX) $(CXXFLAGS) -fPIC -c runtime/heap.cpp -o runtime/heap.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -fPIC -c error.c -o error.o

clean:
	rm -rf *.o *.gch *.dSYM