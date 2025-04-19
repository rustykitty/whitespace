PYTHON = poetry run python

build: setup.py parse_python_tree.c exec_tree.c ws_operation.h setup.py debug.h
	poetry lock
	poetry install --no-root
	$(PYTHON) setup.py build_ext
	$(PYTHON) setup.py build_ext -i

test: whitespace_interpreter_test_template.c parse_python_tree.c exec_tree.c ws_operation.h setup.py debug.h programs/test.wsasm whitespace_interpreter_test.py
	$(PYTHON) whitespace_interpreter_test.py
	gcc -otest -Wall -O0 -g -UNDEBUG $(extra_args) whitespace_interpreter_test.c
