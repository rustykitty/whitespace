PYTHON ?= poetry run python

build: setup.py whitespace_module.c exec_tree.o error.o whitespace.h setup.py utility.h parse_whitespace.o
	poetry install --no-root
	rm -rf build/
	CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS) exec_tree.o error.o parse_whitespace.o" \
	$(PYTHON) setup.py build_ext -i 

exec_tree.o: exec_tree.c exec_tree.h whitespace.h
	$(CC) $(CFLAGS) -fPIC -c exec_tree.c -o exec_tree.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -fPIC -c error.c -o error.o

parse_whitespace.o: parse_whitespace.c parse_whitespace.h whitespace.h
	$(CC) $(CFLAGS) -fPIC -c parse_whitespace.c -o parse_whitespace.o

whitespace_parser: whitespace_parser.c parse_whitespace.o whitespace.h
	$(CC) $(CFLAGS) -o whitespace_parser whitespace_parser.c exec_tree.o error.o parse_whitespace.o

clean:
	rm -rf build/ *.o **/*.o *.so **/*.so