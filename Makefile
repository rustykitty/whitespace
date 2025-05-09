PYTHON = poetry run python

build: setup.py whitespace_module.c exec_tree.o whitespace.h setup.py utility.h
	poetry install --no-root
	rm -rf build/
	CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS) exec_tree.o" $(PYTHON) setup.py build_ext -i 

exec_tree.o: exec_tree.c exec_tree.h whitespace.h
	$(CC) $(CFLAGS) -c exec_tree.c -o exec_tree.o

clean:
	rm -rf build/ *.o **/*.o *.so **/*.so