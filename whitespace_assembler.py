import sys
import os
from ast import literal_eval

import lark

import utility

PROGRAM_NAME = os.path.basename(__file__)

GRAMMAR_PATH = "whitespace_assembly.lark"

with open(GRAMMAR_PATH, "r") as f:
    parser = lark.Lark(f.read(), start="program", parser='lalr')

class WhitespaceAssemblyTransformer(lark.Transformer):
    def number_literal(self, items):
        return int(items[0].value)

    def label_literal(self, items):
        return items[0].value

    def char_literal(self, items):
        return ord(literal_eval(items[0].value))

    def program(self, items):
        return tuple(
            tuple(item) for item in items
        )

    def statement(self, items):
        items[0] = items[0].data
        return tuple(items)

"""
Compile a program to whitespace
"""
def to_whitespace(prog: tuple[tuple[str, int | str, str | str]]):
    res = ""
    for i in prog:
        res += utility.OPERATIONS[i[0]]
        if len(i) == 2:
            if isinstance(i[1], int): # number/char
                num = (' ' if i[1] >= 0 else '\t') + bin(abs(i[1]))[2:].replace('1', '\t').replace('0', ' ')
                res += num
            else: # label
                res += i[1].replace('1', '\t').replace('0', ' ')
            res += '\n'
    return res

TRANSFORMER = WhitespaceAssemblyTransformer()

def parse(prog):
    return parser.parse(prog)

def transform(tree):
    return TRANSFORMER.transform(tree)

def run(data, filename=None):
    try:
        output = parse(data)
        print(output.data)
        transformed_output = TRANSFORMER.transform(output)
        transformed_output = tuple(
            (
                stmt[0], 
                ''.join(bin(c)[2:]
                    for c in bytearray(stmt[1], 'utf-8')
                )
            ) if len(stmt) == 2 and isinstance(stmt[1], str) else stmt
            for stmt in transformed_output
        )
        ws_code = to_whitespace(transformed_output)
        with open(filename + ".ws", "w") as out:
            out.write(ws_code)
    except lark.exceptions.LarkError as e:
        raise
    except OSError:
        utility.write_error(filename + ".ws", e)