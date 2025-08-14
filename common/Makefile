SUBDIRS = error parse runtime

include ../Makefile.common

build: $(SUBDIRS)

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	rm -rf */*.{a,o}
