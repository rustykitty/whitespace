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
- `wsasmc` - Run the Whitespace Assembly compiler (`whitespace_assembler.py`) to produce a Whitespace source file.
- `wsasmexec` - Directly interpret the Whitespace Assembly code.
- `wsdasm` - Disassemble Whitespace code into Whitespace Assembly.

## Whitespace Assembly

My Whitespace Assembly language syntax is as follows:

- Each line consists of a statement, optionally with an argument. For example, `push 42`.
- An integer or any valid C character literal can be used for a numerical argument.
    - Examples of valid character literals are `'A'`, `'\x41'`, `'\u0041'` (all representing the character `A` or `0x41`).
- All valid identifiers in C can be used for a label argument. This may be subject to change.
    - Currently, the valid syntax for a label argument is like this: `label example_label`
- Comments start with `;` and are ignored.
- Behavior follows that of my interpretation of the Whitespace programming language.

TODO:

- Jump precomputation
