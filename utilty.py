import sys

def write_error(program_name, filename, message):
    return sys.stderr.write(f"{program_name}: {filename}: {message}\n")