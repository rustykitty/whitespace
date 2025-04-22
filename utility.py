import sys
import os

PROGRAM_NAME = os.path.basename(sys.argv[0])

def write_error(filename, message):
    return sys.stderr.write(f"{PROGRAM_NAME}: {filename}: {message}\n")

def open_files_in_argv():
    for filename in sys.argv[1:]:
        if filename == "-":
            yield sys.stdin
        else:
            try:
                fp = open(filename)
            except OSError as e:
                write_error(filename, e.strerror)
                continue
            yield fp

OPERATIONS = {
    "push": "  ",
    "dup": " \n ",
    "copy": " \t ",
    "swap": " \n\t",
    "pop": " \n\n",
    "slide": " \t\n",
    "add": "\t   ",
    "sub": "\t  \t",
    "mul": "\t  \n",
    "div": "\t \t ",
    "mod": "\t \t\t",
    "store": "\t\t ",
    "load": "\t\t\t",
    "label": "\n  ",
    "call": "\n \t",
    "jmp": "\n \n",
    "jz": "\n\t ",
    "jlz": "\n\t\t",
    "ret": "\n\t\n",
    "end": "\n\n\n",
    "outchr": "\t\n  ",
    "outnum": "\t\n \t",
    "inchr": "\t\n\t\n",
    "innum": "\t\n\t\t"
}