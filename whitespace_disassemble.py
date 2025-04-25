import whitespace as ws
import whitespace_assembler as wsasm

import utility

import re

import lark

IDENTIFIER = re.compile(r"[_a-zA-Z][_a-zA-Z0-9]*")

def run(data, filename=None):
    try:
        output = ws.parse(data)
    except lark.exceptions.LarkError as e:
        raise
    res = ''
    for stmt in output:
        res += stmt[0]
        if len(stmt) == 2:
            res += (" " + repr(stmt[1]))
        res += "\n"
    with open(filename + ".wsasm", "w") as fp:
        fp.write(res)