import whitespace as ws
import whitespace_assembler as wsasm

import utility

import re

import lark

IDENTIFIER = re.compile(r"[_a-zA-Z][_a-zA-Z0-9]*")

if __name__ == "__main__":
    for file in utility.open_files_in_argv():
        program = file.read()
        try:
            output = ws.parse(program)
        except lark.exceptions.LarkError as e:
            utility.write_error(file.name, e)
            continue
        res = ''
        for stmt in output:
            res += stmt[0]
            if len(stmt) == 2:
                res += (" " + repr(stmt[1]))
            res += "\n"
        with open(file.name + ".wsasm", "w") as fp:
            fp.write(res)