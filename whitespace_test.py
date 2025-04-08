import json

import whitespace

from lark import Transformer

PROGRAM = "\n  \t \n  \t \n"

if __name__ == '__main__':
    output = whitespace.parser.parse(PROGRAM)
    print(output.pretty())
    print(whitespace.NumberTransformer().transform(output).pretty())