import re
import json

import whitespace

from lark import Transformer

PROGRAM_CHAR = re.compile(r'[ \t\n]')

def minify(program: str):
    return ''.join(m.group(0) for m in PROGRAM_CHAR.finditer(program))

PROGRAM = minify(open('hello.ws').read())

if __name__ == '__main__':
    output = whitespace.parse(PROGRAM)

    open('dump.asm', 'w').write(whitespace.LiteralTransformer().transform(whitespace.parser.parse(PROGRAM)).pretty())
    # print(output.pretty())
    whitespace.interpret_c_wrapper(output)