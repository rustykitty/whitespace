PYTHON = poetry run python

build: setup.py parse_python_tree.c exec_tree.c ws_operation.h setup.py debug.h
	rm -rf build/
	$(PYTHON) setup.py build_ext
	$(PYTHON) setup.py build_ext -i