SUBDIRS = error parse runtime

MAKEFILE_DIR = $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

export CFLAGS += -I$(MAKEFILE_DIR)/include
export CXXFLAGS += -I$(MAKEFILE_DIR)/include

build: $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

clean:
	rm -rf */*.{a,o}