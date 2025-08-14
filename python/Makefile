PYTHON ?= uv run python

.PHONY: build clean common install_dependencies

LDFLAGS += ../common/parse/parse.a ../common/runtime/runtime.a

build: clean setup.py whitespace_module.c common install_dependencies
	$(PYTHON) setup.py build_ext -i

common:
	$(MAKE) -C ../common parse runtime

install_dependencies:
	uv sync

clean:
	rm -rf build/
	rm -rf *.so
