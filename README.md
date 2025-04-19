# Whitespace Interpreter and "Whitespace Assembly" compiler

A project I made because I was bored.

My Python program follows Whitespace v0.3, as described on the [Esolangs Wiki](https://esolangs.org/wiki/Whitespace).

The programs have only been tested with macOS 15.3.2 and Python 3.12.9. If there are issues with running on other platforms, please let me know, preferrably by opening an issue.

Additionally, there may be bugs, especially around the Whitespace interpreter's handling of labels, jumps, and heap access (I have not done testing for those). Please open an issue for these as well.

Whitespace or Whitespace Assembly programs included in this repository are either taken from Wikipedia, taken from the Esolangs Wiki, or written by myself.

## Instructions

Ensure that Poetry is installed. You can find instructions at <https://python-poetry.org/docs/>.

`git clone` the repository. You will need to run `make` the to compile the C extension code prior to running it the first time, and recompile every time it is changed.

A number of shell scripts are available to allow for running the various Python scripts with less typing.

These include:

- `ws` - Run the Whitespace interpreter (`whitespace.py`)
- `wsasmc` - Run the Whitespace Assembly compiler (`whitespace_assembler.py`)
- `wsasmexec` - Runs the Whitespace Assembly code directly.
- `wsdasm` - Disassemble Whitespace code into Whitespace Assembly

TODO:

- Make labels more efficient
