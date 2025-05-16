import sys

from utility import write_error, PROGRAM_NAME

import lark

HELP_TEXT = f"""
Usage: {PROGRAM_NAME} <command> [file...]

For commands which read from files given as command-line arguments, stdin is
assumed if no file arguments are given.

Commands:
- ws, whitespace    interpret and execute whitespace files
- wsasmc            compile Whitespace Assembly files to whitespace
- wsasmexec         interpret and execute Whitespace Assembly files, skipping compilation
- wsdasm            disassemble whitespace files into Whitespace Assembly
"""

def main():
    if len(sys.argv) < 2:
        write_error(f"usage: {PROGRAM_NAME} <command> [file...]")
        sys.exit(1)

    mode = sys.argv[1]
    if mode in ("ws", "whitespace"):
        import whitespace as ws
        run = ws.run
    elif mode == "wsasmc":
        import whitespace_assembler as wsasmc
        run = wsasmc.run
    elif mode == "wsasmexec":
        import whitespace_assemble_and_run as wsasmexec
        run = wsasmexec.run
    elif mode == "wsdasm":
        import whitespace_disassemble as wsdasm
        run = wsdasm.run
    else:
        sys.stderr.write(HELP_TEXT)
        sys.exit(1)

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