# Ish Compiler

Ish is a small compiler project implemented in C. It is being developed as a practical compiler-construction showcase inspired by the classic compiler pipeline: source reader, lexer, parser, semantic analysis, intermediate representation, and execution/code generation.

The current codebase contains the original reader, scanner, and parser stages. The next milestones are to stabilize the front end, define the language precisely, add an AST, add semantic analysis, and then add an IR-backed execution or code-generation path.

## Current Features

- Dynamic source reader/buffer.
- Token scanner for comments, identifiers, literals, delimiters, arithmetic operators, assignment, and keywords.
- Recursive-descent parser for the current showcase subset: function declarations/calls, variable declarations, assignments, input, print, and simple `while` blocks.
- Symbol table checks for duplicate declarations and undefined variable/function usage.
- Visual Studio project files.
- Portable CMake build for GCC, Clang, and MSVC.

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

The executable is named `ish`.

## Usage

```sh
./build/ish r examples/hello.ish
./build/ish s examples/hello.ish
./build/ish p examples/hello.ish
./build/ish p examples/volume.ish
./build/ish p examples/factorial.ish
./build/ish p examples/semantic-error.ish
```

Modes:

- `r`: reader/buffer diagnostics.
- `s`: scanner/token diagnostics.
- `p`: parser diagnostics.

## Roadmap

- Replace expression skipping with AST-building expression parsing.
- Add AST construction and AST dumps.
- Expand semantic analysis with type checking and function parameter validation.
- Add an intermediate representation.
- Add a bytecode VM or C backend.
- Add automated unit, golden, and end-to-end tests.

## Repository Layout

- `*.c`, `*.h`: current compiler implementation.
- `examples/`: sample Ish programs.
- `docs/`: language and compiler pipeline documentation.
- `CMakeLists.txt`: portable build definition.
- `Ish.sln`, `Ish.vcxproj`: Visual Studio project files.
