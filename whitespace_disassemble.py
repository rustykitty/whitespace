import whitespace as ws
import whitespace_assembly as wsasm

import utility

import re

IDENTIFIER = re.compile(r"[_a-zA-Z][_a-zA-Z0-9]*")

if __name__ == "__main__":
    for file in utility.open_files_in_argv():
        program = file.read()
        output = ws.parse(program)
        res = ''
        for stmt in output:
            res += stmt[0]
            if len(stmt) == 2:
                res += (" " + repr(stmt[1]))
            res += "\n"
        with open(file.name + ".wsasm", "w") as fp:
            fp.write(res)