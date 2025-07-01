makefile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_path := $(patsubst %/,%,$(dir $(makefile_path)))
project_root := $(patsubst %/,%,$(dir $(current_path)))

PYTHON ?= uv run python

build: setup.py whitespace_module.c ../c setup.py
	uv sync
	rm -rf build/
	CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS) $(wildcard $(project_root)/common/*.o)" \
	$(PYTHON) setup.py build_ext -i

clean:
	rm -rf build/
	rm -rf *.so
