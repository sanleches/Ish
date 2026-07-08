# Compiler Pipeline

Ish is being evolved toward a conventional multi-stage compiler pipeline.

```text
source file
  -> reader
  -> scanner / lexer
  -> parser
  -> AST
  -> semantic analyzer
  -> intermediate representation
  -> optimizer
  -> VM or code generator
```

## Current Status

- Reader: present, needs cleanup and stronger tests.
- Scanner: deterministic tokenization is present; location-aware diagnostics are still planned.
- Parser: recursive-descent parsing covers the current examples; AST construction is still planned.
- Semantic analysis: initial scoped symbol table checks are present; type checking is planned.
- IR: planned.
- Optimizer: planned.
- Runtime/backend: planned.

## Near-Term Engineering Priorities

- Keep the build portable with CMake and Visual Studio.
- Add repeatable tests before large rewrites.
- Prefer small, verifiable compiler phases over hidden global state.
- Emit stable dump formats for tokens, AST, and IR so behavior can be tested with golden files.
