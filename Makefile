SUBDIRS := c python

all:
	make -C c
	make -C python


clean:
	rm -rf python/build/ out/ **.o **.so **.dSYM/ **.gch

.PHONY: all $(SUBDIRS)