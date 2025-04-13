import sys
import re
from typing import Callable, Any
import os

import lark

from utilty import write_error

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
        # print(items)
        return [
            (item.data, tuple(item.children)) for item in items
        ]

def parse(prog: str):
    return AllTransformer().transform(parser.parse(prog))

op_name_to_num = {
    x: i for (i, x) in enumerate(
        (
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
    )
}

def interpret_c_wrapper(prog: tuple[str, tuple[int] | tuple[()]]):
    """Wrapper for C function"""
    prog_list = []
    for i in range(len(prog)):
        assert 1 <= len(prog[i]) <= 2
        op = prog[i][0]
        if op not in op_name_to_num:
            raise ValueError(f"Expected one of {op_name_to_num.keys()}, got {op} instead "
                             "(at index {i} of prog)")
        if len(prog[i]) == 1:
            prog_list.append( (op_name_to_num[op],) )
        else:
            # length 2
            prog_list.append( (op_name_to_num[op], *prog[i][1]) ) # arg
    prog_list = tuple(prog_list)
    # call c function now
    parse_python_tree.parse_python_tree(prog_list)

PROGRAM_CHARS = ' \t\n'

def minify(program: str):
    return ''.join(c for c in program if c in PROGRAM_CHARS)

if __name__ == "__main__":
    for file in sys.argv[1:]:
        try:
            f = open(file, "r")
        except IOError as e:
            write_error(PROGRAM_NAME, file, e.strerror)
            continue
        try:
            output = parse(minify(f.read()))
            interpret_c_wrapper(output)
        except lark.exceptions.LarkError as e:
            write_error(PROGRAM_NAME, file, e)