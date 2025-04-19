import whitespace as ws
import whitespace_assembler as wsasm

PROGRAM = open('test.wsasm').read()

TEMPLATE = open("whitespace_interpreter_test_template.c").read()

if __name__ == '__main__':
    output = wsasm.parse(PROGRAM)
    t_out = wsasm.transform(output)
    print(t_out)
    statements = []
    for stmt in t_out:
        op = str(stmt[0])
        if len(stmt) == 2:
            arg = stmt[1]
            if isinstance(arg, int): # number literal
                statements.append("{ %s, .num = %d }," % (f"WS_{op.upper()}", arg))
            elif isinstance(arg, str):
                statements.append("{ %s, .label = \"%s\" }," % (f"WS_{op.upper()}", arg))
            else:
                raise ValueError(f"unrecognized type for arg: {type(arg)}")
        else:
            statements.append("{ %s }," % f"WS_{op.upper()}")
    with open("whitespace_interpreter_test.c", "w") as fp:
        text = TEMPLATE.replace("/* INSERT STATEMENTS HERE */", '\n'.join(statements))
        fp.write(text)