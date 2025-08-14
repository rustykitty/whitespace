SUBDIRS = c common python

include Makefile.common

build: $(SUBDIRS)

.PHONY: $(SUBDIRS) clean build
$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
