import whitespace as ws
import whitespace_assembler as wsasm
import utility

import lark

def run(data, filename=None):
    try:
        output = wsasm.parse(data)
        transformed_output = wsasm.TRANSFORMER.transform(output)
        ws.interpret_c_wrapper(transformed_output)
    except lark.exceptions.LarkError as e:
        raise