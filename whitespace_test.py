import whitespace

PROGRAM_CHARS = ' \t\n'

def minify(program: str):
    return ''.join(c for c in program if c in PROGRAM_CHARS)

PROGRAM = minify(open('hello.ws').read())

if __name__ == '__main__':
    output = whitespace.parse(PROGRAM)
    whitespace.interpret_c_wrapper(output)