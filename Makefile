PYTHON = poetry run python

build: setup.py whitespace_module.c exec_tree.c whitespace.h setup.py utility.h
	poetry install --no-root
	rm -rf build/
	$(PYTHON) setup.py build_ext -i $(CFLAGS)

build_debug: 
	make build CFLAGS='-g -UNDEBUG'
