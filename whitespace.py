import sys
import re
import os

import lark

import utility

import whitespace_module

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
        return '__auto_' + utility.compress_label(items[0], "\t", " ")

class AllTransformer(LiteralTransformer):

    def program(self, items):
        return tuple(
            (item.data, *item.children) for item in items
        )

OPERATIONS = tuple(utility.OPERATIONS.keys())

def parse(prog: str):
    return AllTransformer().transform(parser.parse(''.join(x for x in prog if x in PROGRAM_CHARS)))

def interpret_c_wrapper(prog: tuple[str, int | str] | tuple[str] ):
    """Wrapper for C function"""
    prog = tuple(
        (OPERATIONS.index(x[0]), *(x[1:])) for x in prog
    )
    # call c function now
    whitespace_module.parse_and_exec(prog)

PROGRAM_CHARS = ' \t\n'

def minify(program: str):
    return ''.join(c for c in program if c in PROGRAM_CHARS)

def run(data, filename=None):
    try:
        output = parse(data)
        interpret_c_wrapper(output)
    except lark.exceptions.LarkError as e:
        raise
