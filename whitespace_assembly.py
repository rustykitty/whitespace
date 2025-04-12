import lark

GRAMMAR_PATH = "whitespace_assembly.lark"

with open(GRAMMAR_PATH, "r") as f:
    parser = lark.Lark(f.read(), start="program")

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
