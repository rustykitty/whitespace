PYTHON = poetry run python

build: setup.py parse_python_tree.c exec_tree.c ws_operation.h setup.py debug.h
	poetry install --no-root
	rm -rf build/
	$(PYTHON) setup.py build_ext -i $(CFLAGS)

build_debug: 
	make build CFLAGS='-g -UNDEBUG'

test: whitespace_interpreter_test_template.c parse_python_tree.c exec_tree.c ws_operation.h setup.py debug.h programs/test.wsasm whitespace_interpreter_test.py
	$(PYTHON) whitespace_interpreter_test.py
	gcc -otest -Wall -O0 -g -UNDEBUG $(extra_args) whitespace_interpreter_test.c
