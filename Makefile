PYTHON = poetry run python

build: setup.py parse_python_tree.c exec_tree.c ws_operation.h setup.py utility.h
	poetry install --no-root
	rm -rf build/
	$(PYTHON) setup.py build_ext -i $(CFLAGS)

build_debug: 
	make build CFLAGS='-g -UNDEBUG'
