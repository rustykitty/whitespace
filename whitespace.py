import sys
import re
from typing import Callable, Any
import os

import lark

from utility import write_error

import parse_python_tree

PROGRAM_NAME = os.path.basename(__file__)

GRAMMAR_PATH = "whitespace.lark"

with open(GRAMMAR_PATH, "r") as f:
    parser = lark.Lark(f.read(), start="program")

NUMBER_PATTERN = re.compile("^([ \t])([ \t]+)\n$")

def whitespace_number(num: str) -> int:
    match: re.Match = NUMBER_PATTERN.match(num)
    if not match:
        raise ValueError(f"Invalid number: {num}")
    res = 0
    for c in match.group(2):
        res <<= 1
        if c == "\t":
            res += 1
    positive = match.group(1) == " "
    return res if positive else -res

class LiteralTransformer(lark.Transformer):
    def number_literal(self, items):
        return whitespace_number(items[0].value)
    
    def label_literal(self, items):
        return items[0].replace('\t', '1').replace(' ', '0')

class AllTransformer(lark.Transformer):
    def number_literal(self, items):
        return whitespace_number(items[0].value)
    
    def label_literal(self, items):
        return items[0].replace('\t', '1').replace(' ', '0')
    
    def program(self, items):
        return tuple(
            (item.data, *item.children) for item in items
        )

def parse(prog: str):
    return AllTransformer().transform(parser.parse(''.join(x for x in prog if x in PROGRAM_CHARS)))

ops = (
            "push",
            "dup",
            "copy",
            "swap",
            "pop",
            "slide",
            "add",
            "sub",
            "mul",
            "div",
            "mod",
            "store",
            "load",
            "label",
            "jump",
            "jz",
            "jlz",
            "call",
            "ret",
            "end",
            "outchr",
            "outnum",
            "inchr",
            "innum"
        )

def interpret_c_wrapper(prog: tuple[str, int | str] | tuple[str] ):
    """Wrapper for C function"""
    prog = tuple(
        (ops.index(x[0]), *(x[1:])) for x in prog
    )
    # call c function now
    parse_python_tree.parse_python_tree(prog)

PROGRAM_CHARS = ' \t\n'

def minify(program: str):
    return ''.join(c for c in program if c in PROGRAM_CHARS)

if __name__ == "__main__":
    for file in sys.argv[1:]:
        try:
            f = open(file)
        except IOError as e:
            write_error(PROGRAM_NAME, file, e.strerror)
            continue
        try:
            output = parse(f.read())
            interpret_c_wrapper(output)
        except lark.exceptions.LarkError as e:
            write_error(PROGRAM_NAME, file, e)