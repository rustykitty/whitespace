import sys
import os

PROGRAM_NAME = os.path.basename(sys.argv[0])

def write_error(program_name, filename, message):
    return sys.stderr.write(f"{program_name}: {filename}: {message}\n")

def open_files_in_argv():
    for filename in sys.argv[1:]:
        if filename == "-":
            yield sys.stdin
        else:
            try:
                fp = open(filename)
            except OSError as e:
                write_error(PROGRAM_NAME, filename, e.strerror)
                continue
            yield fp
