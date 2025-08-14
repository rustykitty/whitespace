whitespace_parser:
	$(MAKE) -C ../common parse runtime
	$(CC) $(CFLAGS) -o whitespace_parser whitespace_parser.c ../common/parse/parse.a ../common/runtime/runtime.a

.PHONY: clean
clean:
	rm -f whitespace_parser
