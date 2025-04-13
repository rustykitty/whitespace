import sys
import os

import lark

from utilty import write_error

PROGRAM_NAME = os.path.basename(__file__)

GRAMMAR_PATH = "whitespace_assembly.lark"

with open(GRAMMAR_PATH, "r") as f:
    parser = lark.Lark(f.read(), start="program", parser='lalr')

class WhitespaceAssemblyTransformer(lark.Transformer):
    def number_literal(self, items):
        return int(items[0].value)
    
    def label_literal(self, items):
        return ''.join(bin(c)[2:]
                       # .replace('1', '\t').replace('0', ' ')
                       for c in bytearray(items[0].value, 'utf-8')
                       )

    def char_literal(self, items):
        return ord(eval(items[0].value)) # dangerous

    def program(self, items):
        return tuple(
            tuple(item) for item in items
        )

    def statement(self, items):
        items[0] = items[0].data
        return tuple(items)

def to_whitespace(prog: tuple[tuple[str, int | str, str | str]]):
    res = ""
    for i in prog:
        res += INSTRUCTIONS[i[0]]
        if len(i) == 2:
            if isinstance(i[1], int): # number/char
                num = (' ' if i[1] >= 0 else '\t') + bin(abs(i[1]))[2:].replace('1', '\t').replace('0', ' ')
                res += num
            else: # label
                res += i[1].replace('1', '\t').replace('0', ' ')
            res += '\n'
    return res

INSTRUCTIONS = {
    "push": "  ",
    "dup": " \n ",
    "copy": " \t ",
    "swap": " \n\t",
    "pop": " \n\n",
    "slide": " \t\n",
    "add": "\t   ",
    "sub": "\t  \t",
    "mul": "\t  \n",
    "div": "\t \t ",
    "mod": "\t \t\t",
    "store": "\t\t ",
    "load": "\t\t\t",
    "label": "\n  ",
    "call": "\n \t",
    "jmp": "\n \n",
    "jz": "\n\t ",
    "jlz": "\n\t\t",
    "ret": "\n\t\n",
    "end": "\n\n\n",
    "outchr": "\t\n  ",
    "outnum": "\t\n \t",
    "inchr": "\t\n\t\n",
    "innum": "\t\n\t\t"
}

if __name__ == "__main__":
    for file in sys.argv[1:]:
        try:
            f = open(file, "r")
        except IOError as e:
            write_error(PROGRAM_NAME, file, e.strerror)
            continue
        try:
            prog = f.read()
            output = parser.parse(prog)
            transformed_output = WhitespaceAssemblyTransformer().transform(output)
            ws_code = to_whitespace(transformed_output)
            with open(file + ".ws", "w") as out:
                out.write(ws_code)
        except lark.exceptions.LarkError as e:
            write_error(PROGRAM_NAME, file, e)