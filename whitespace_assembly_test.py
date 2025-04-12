import whitespace_assembly as wsasm

if __name__ == '__main__':
    PROGRAM = open('hello_loop.wsasm').read()
    output = wsasm.parser.parse(PROGRAM)
    print(output.pretty())
    print(wsasm.WhitespaceAssemblyTransformer().transform(output))