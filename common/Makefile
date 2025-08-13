SUBDIRS = error parse runtime

build: $(SUBDIRS)

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	rm -rf */*.{a,o}
