SUBDIRS := $(dir $(wildcard */Makefile))

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
:
clean:
	@for dir in $(SUBDIRS); do \
		if $(MAKE) -C $$dir --dry-run clean > /dev/null 2>&1; then \
			echo "$(MAKE) -C $$dir clean"; \
			$(MAKE) -C $$dir clean; \
		fi; \
	done

.PHONY: all $(SUBDIRS)
