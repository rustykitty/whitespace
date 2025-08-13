import sys

from utility import write_error, PROGRAM_NAME

# pre-import for performance

import lark

import whitespace as ws
import whitespace_assembler as wsasmc
import whitespace_assemble_and_run as wsasmexec
import whitespace_disassemble as wsdasm

HELP_TEXT = f"""
Usage: {PROGRAM_NAME} <command> [file...]

For commands which read from files given as command-line arguments, stdin is
assumed if no file arguments are given.

Commands:
- ws, whitespace    interpret and execute Whitespace files
- wsasmc            compile Whitespace Assembly files to Whitespace
- wsasmexec         interpret and execute Whitespace Assembly files, without compiling
- wsdasm            disassemble Whitespace files into Whitespace Assembly
"""

def main():
    if len(sys.argv) < 2:
        write_error(f"usage: {PROGRAM_NAME} <command> [file...]\ntry '{PROGRAM_NAME} help'")
        sys.exit(1)

    mode = sys.argv[1]
    if mode == "help":
        print(HELP_TEXT)
        sys.exit(0)
    if mode in ("ws", "whitespace"):
        run = ws.run
    elif mode == "wsasmc":
        run = wsasmc.run
    elif mode == "wsasmexec":
        run = wsasmexec.run
    elif mode == "wsdasm":
        run = wsdasm.run

    for file in sys.argv[2:]:
        try:
            f = open(file)
        except IOError as e:
            write_error(e.strerror, file)
            continue
        try:
            run(f.read(), file)
        except lark.exceptions.LarkError as e:
            write_error(e, file)


if __name__ == "__main__":
    main()