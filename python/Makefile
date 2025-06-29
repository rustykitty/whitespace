PYTHON ?= uv run python

build: setup.py whitespace_module.c ../c setup.py
	uv sync
	rm -rf build/
	CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS) $(wildcard $(shell dirname ${CURDIR})/c/*.o)" \
	$(PYTHON) setup.py build_ext -i
