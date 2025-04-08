import re

import lark

GRAMMAR_PATH = "grammar.lark"

with open(GRAMMAR_PATH, "r") as f:
    parser = lark.Lark(f.read())

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


class NumberTransformer(lark.Transformer):
    def number_literal(self, items):
        return whitespace_number(items[0].value)
    
    def label_literal(self, items):
        return items[0].replace('\t', '1').replace(' ', '0')