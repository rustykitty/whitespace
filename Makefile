SUBDIRS = c common python

MAKEFILE_DIR = $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

export PROJECT_ROOT = $MAKEFILE_DIR

export CFLAGS += -I$(MAKEFILE_DIR)/include -lstdc++
export CXXFLAGS += -I$(MAKEFILE_DIR)/include

build: $(SUBDIRS)

.PHONY: $(SUBDIRS) clean build
$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
