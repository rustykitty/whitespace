PYTHON ?= uv run python

include ../Makefile.common

.PHONY: build clean common install_dependencies test

LDFLAGS += ../common/parse/parse.a ../common/runtime/runtime.a

build: setup.py whitespace_module.c common install_dependencies
	$(MAKE) clean
	$(PYTHON) setup.py build_ext -i

common:
	$(MAKE) -C ../common parse runtime

install_dependencies:
	uv sync

clean:
	rm -rf build/
	rm -rf *.so

test:
	$(PYTHON) -m unittest discover -s tests