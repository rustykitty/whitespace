import whitespace

PROGRAM = whitespace.minify(open('hello.ws').read())

if __name__ == '__main__':
    output = whitespace.parse(PROGRAM)
    whitespace.interpret_c_wrapper(output)