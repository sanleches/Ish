# Ish Language

This document describes the target direction for Ish. The implementation is being aligned with this specification incrementally.

## Goals

Ish is intended to be a compact teaching language for demonstrating compiler phases. The syntax should be small enough to implement by hand, but rich enough to show lexical analysis, parsing, symbol tables, semantic checks, intermediate representation, and execution.

## Lexical Elements

- Identifiers: names beginning with a letter or underscore, followed by letters, digits, or underscores.
- Integers: decimal integer literals.
- Floats: decimal floating-point literals.
- Strings: quoted string literals.
- Comments: `# ... #` block comments and `## ...` line comments.

## Planned Keywords

- `funk`: function declaration.
- `v`: variable declaration.
- `ish_intg`: integer type.
- `ish_flop`: floating-point type.
- `ish_thread`: string type.
- `if`, `else`, `while`, `for`, `return`: control flow.
- `print`, `input`: built-in I/O.
- `TRUE`, `FALSE`: boolean literals.

## Planned Grammar

```ebnf
program      = declaration* EOF ;
declaration  = functionDecl | varDecl | statement ;
functionDecl = "funk" identifier "(" parameters? ")" block ;
parameters   = parameter ("," parameter)* ;
parameter    = type identifier ;
varDecl      = "v" type identifier ("=" expression)? ";" ;
type         = "ish_intg" | "ish_flop" | "ish_thread" ;
statement    = printStmt | inputStmt | ifStmt | whileStmt | returnStmt | exprStmt | block ;
block        = "{" declaration* "}" ;
printStmt    = "print" "(" arguments? ")" ";" ;
inputStmt    = "input" "(" identifierList? ")" ";" ;
returnStmt   = "return" expression? ";" ;
exprStmt     = expression ";" ;
expression   = assignment ;
assignment   = identifier "=" assignment | equality ;
```

## Implemented Front-End Subset

The current parser accepts the showcase examples under `examples/`:

- Function declarations with empty parameter lists.
- Function calls.
- Variable declarations.
- Assignments.
- `print(...)` and `input(...)` statements.
- Simple `while (...) { ... }` blocks.
- Duplicate declaration and undefined symbol checks.

Expressions are represented as AST nodes with precedence for relational, additive, multiplicative, primary, and parenthesized expressions. Assignment remains statement-level in the current parser.

## Diagnostics

Diagnostics should include file name, line, column, severity, and a clear message.

```text
examples/bad.ish:4:12: error: undefined variable 'total'
```
