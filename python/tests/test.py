import unittest

class TestWhitespaceParser(unittest.TestCase):
    def test_import_parser(self):
        from whitespace_module import exec_tree, parse_and_exec
    
    def test_parse_and_exec(self):
        from whitespace_module import parse_and_exec
        parse_and_exec("\n\n\n") # only one END instruction