runtime.a: heap.o runtime.o
	$(MAKE) -C ../error error.o
	ar rcs runtime.a heap.o runtime.o ../error/error.o

heap.o: heap.cpp
	$(CXX) $(CXXFLAGS) -c heap.cpp -o heap.o

runtime.o: runtime.c
	$(CC) $(CFLAGS) -c runtime.c -o runtime.o
