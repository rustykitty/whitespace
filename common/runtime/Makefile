runtime.a: heap.o runtime.o
	ar rcs runtime.a heap.o runtime.o

heap.o: heap.cpp
	$(CXX) $(CXXFLAGS) -c heap.cpp -o heap.o

runtime.o: runtime.c
	$(CC) $(CFLAGS) -c runtime.c -o runtime.o
