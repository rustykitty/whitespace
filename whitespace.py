import sys
import re
from typing import Callable, Any

import lark

import parse_python_tree

GRAMMAR_PATH = "grammar.lark"

def pop_two(stack: list) -> tuple:
    return (stack.pop(), stack.pop())[::-1]

INSTRUCTIONS: dict[str, Callable[[list, list, Any], None]] = {
    "push": lambda callstack, stack, arg: stack.append(arg),
    "dup": lambda callstack, stack, arg: stack.append(stack[-1]),
    "copy": lambda callstack, stack, arg: stack.append(stack[-arg + 1]),
    "swap": lambda callstack, stack, arg: ((two := (stack.pop(), stack.pop())), stack.extend(two)),
    "pop": lambda callstack, stack, arg: stack.pop(),
    "slide": lambda callstack, stack, arg: ((top := stack[-1]), [stack.pop() for _ in range(arg - 1)], stack.append(top)),
    "add": lambda callstack, stack, arg: stack.append(stack.pop() + stack.pop()),
    "sub": lambda callstack, stack, arg: ((op := pop_two(stack)), stack.append(op[0] - op[1])),
    "mul": lambda callstack, stack, arg: stack.append(stack.pop() * stack.pop()),
    "div": lambda callstack, stack, arg: ((op := pop_two(stack)), stack.append(op[0] // op[1])),
    "mod": lambda callstack, stack, arg: ((op := pop_two(stack)), stack.append(op[0] % op[1])),
    "store": lambda callstack, stack, arg: ((op := pop_two(stack)), heap.update({op[0]: op[1]})),
    "load": lambda callstack, stack, arg: stack.append(heap.get(arg)),
    "label": lambda callstack, stack, arg: None, # TODO: flow control
    "jump": lambda callstack, stack, arg: None, # TODO
    "jz": lambda callstack, stack, arg: None, # TODO
    "jlz": lambda callstack, stack, arg: None, # TODO
    "call": lambda callstack, stack, arg: None, # TODO
    "ret": lambda callstack, stack, arg: None, # TODO
    "end": lambda callstack, stack, arg: None, # TODO
    "outchr": lambda callstack, stack, arg: sys.stdout.write(chr(stack[-1])),
    "outnum": lambda callstack, stack, arg: print(stack[-1]),
    "inchr": lambda callstack, stack, arg: stack.append(ord(sys.stdin.read(1))),
    "innum": lambda callstack, stack, arg: stack.append(int(sys.stdin.readline())),
}

heap: dict[int, int] = {}

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

def interpret(prog: str):
    instructions = LiteralTransformer().transform(parser.parse(prog))

    for instruction in instructions:
        op, arg = instruction
        if type(arg) == tuple:
            arg = arg[0] if arg else None
        if op == "end": 
            break
        if op in INSTRUCTIONS:
            INSTRUCTIONS[op](callstack=[], stack=[], arg=arg)
        else:
            assert False

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
    # call c function now
    parse_python_tree.parse_python_tree(tuple(prog_list))