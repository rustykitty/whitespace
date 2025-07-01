import whitespace_module

PROGRAM_CHARS = ' \t\n'

def minify(program: str):
    return ''.join(c for c in program if c in PROGRAM_CHARS)

def run(data, filename=None):
    try:
        whitespace_module.parse_and_exec(data)
    except RuntimeError as e:
        print(repr(e))
        raise
