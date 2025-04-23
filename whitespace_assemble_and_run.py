import whitespace as ws
import whitespace_assembler as wsasm
import utility

import lark

if __name__ == "__main__":
    for f in utility.open_files_in_argv():
        try:
            prog = f.read()
            output = wsasm.parse(prog)
            transformed_output = wsasm.TRANSFORMER.transform(output)
            ws.interpret_c_wrapper(transformed_output)
        except lark.exceptions.LarkError as e:
            utility.write_error(f.name, e)