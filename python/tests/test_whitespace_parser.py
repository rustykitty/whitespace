import unittest

class TestWhitespace(unittest.TestCase):

    def setUp(self):
        import whitespace
        self.whitespace = whitespace
    
    def test_minify(self):
        self.assertEqual(self.whitespace.minify("A\tB\nC D"), "\t\n ")

    def test_run(self):
        self.whitespace.run("\n\n\n")

class TestWhitespaceAssembler(unittest.TestCase):

    def setUp(self):
        import whitespace_assembler
        self.whitespace_assembler = whitespace_assembler

    def test_assemble(self):
        code = "push 1\npush 1\nadd\noutnum\nend"
        self.whitespace_assembler.run(code)

class TestWhitespaceAssembleAndRun(unittest.TestCase):

    def setUp(self):
        import whitespace_assemble_and_run
        self.whitespace_assemble_and_run = whitespace_assemble_and_run

    def test_run(self):
        code = "push 1\npush 2\nadd\noutnum\nend"
        self.whitespace_assemble_and_run.run(code)

# class TestWhitespaceDisassemble(unittest.TestCase):

#     def setUp(self):
#         import whitespace_disassemble
#         self.whitespace_disassemble = whitespace_disassemble

#     def test_disassemble(self):
#         code = "\n\n\n" 
#         assembly = self.whitespace_disassemble.run(code)
#         self.assertIn("end", assembly)