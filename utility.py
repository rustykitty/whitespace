import sys
import os
import base64

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

def zeroes_and_ones_to_bytes(s: str, zero: str = "0", one: str = "1"):
    if s and s[-1] == '\n':
        s = s[:-1]
    s = str(s).replace(zero, "0").replace(one, "1")

    l = len(s)
    res = b''
    while len(s) >= 8:
        res += bytes((int(s[:8], 2),))
        s = s[8:]
    if s:
        res += bytes((int(s.zfill(8), 2),))
    return res

def bytes_to_zeroes_and_ones(s: bytes, zero: str = "0", one: str = "1"):
    return ''.join(
        bin(c if isinstance(c, int) else ord(c))[2:].replace(zero, "0").replace(one, "1") for c in s
    )

def compress_label(s: str, zero: str = "0", one: str = "1"):
    return base64.b32encode(zeroes_and_ones_to_bytes(s, zero, one)).decode("ascii").replace("=", "_")

def decompress_label(s: str):
    s = s.replace("_", "=").encode('ascii')
    return bytes_to_zeroes_and_ones(base64.b32decode(s))