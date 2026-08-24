# Compiler from Scratch

A compiler implemented entirely from scratch in C as part of a Computer Science compiler construction project.

The project implements the core stages of a compiler, including lexical analysis, parsing, semantic analysis, symbol tables, function handling, and assembly code generation for a custom programming language.

The final compiler supports variables, arithmetic expressions, control structures, functions, function calls, global and local scopes, and assembly code generation.

---

## Features

- Lexical analysis and tokenization
- Hand-written recursive-descent parser
- Semantic analysis
- Symbol table implementation
- Variable declarations
- Arithmetic expressions
- Input/output operations
- Conditional statements (`if` / `else`)
- `while` loops
- Function declarations and calls
- Function prototypes
- Global and local scopes
- Function parameter handling
- Control-flow generation
- Assembly code generation

---

## Compiler Architecture

The compiler is organized into several stages:

    Source Code
        ↓
    Lexical Analysis
        ↓
    Parsing
        ↓
    Semantic Analysis
        ↓
    Code Generation
        ↓
    Assembly

### Lexical Analysis

The lexical analyzer reads the source program and converts it into a sequence of tokens that are consumed by the parser.

### Parsing

The parser is implemented as a **hand-written recursive-descent parser**, with grammar productions represented directly by C functions.

The parser uses a lookahead token to determine which grammar rule should be applied and validates the syntactic structure of the source program.

Arithmetic expressions are parsed using separate grammar functions to handle operator precedence and grouping.

### Semantic Analysis

Semantic checks are performed during the compilation process, including:

- Variable declaration checking
- Symbol lookup
- Global and local scope handling
- Function declaration validation
- Function prototype validation
- Function parameter validation
- Type checking for variables and function parameters

### Symbol Tables

The compiler maintains symbol tables for:

- Variables
- Functions
- String literals

Global and local variable scopes are supported, allowing symbols to be resolved according to their current scope.

### Code Generation

The compiler generates assembly code for the supported language constructs, including:

- Variable operations
- Arithmetic expressions
- Input/output operations
- Conditional branches
- Loops
- Function calls
- Function control flow
- Labels and jumps

---

## Language Features

The implemented language supports:

- Variable declarations
- Arithmetic expressions
- Input/output
- `if` / `else`
- `while` loops
- Functions
- Function calls
- Function prototypes
- Global scope
- Local scope

---

## Functions and Control Flow

The final stage of the project focused on adding support for functions and function calls.

This included:

- Function declarations
- Function prototypes
- Function parameter handling
- Function code generation
- Function calls
- Function labels
- Control-flow jumps
- Function completion and return control flow
- Validation of function implementations against their prototypes

---

## Educational Context

This project was developed as part of the **Compiler Construction** course at the **Federal University of Mato Grosso (UFMT)**.

The main objective was to gain a practical understanding of compiler internals by implementing the main stages of a compiler manually, from lexical analysis and parsing to semantic analysis and assembly code generation.

Rather than relying on a compiler framework, the project was implemented from scratch to understand how the individual components of a compiler work together.

---

## Technologies

- C
- Compiler construction
- Lexical analysis
- Recursive-descent parsing
- Semantic analysis
- Symbol tables
- Assembly code generation

---

## What I Learned

Through this project, I gained practical experience with:

- Designing and implementing a compiler pipeline
- Translating grammar rules into parser logic
- Working with tokens and lookahead
- Implementing symbol tables and scope handling
- Performing semantic validation
- Handling functions and function parameters
- Generating control flow
- Translating high-level language constructs into assembly instructions
- Understanding how different compiler stages interact

---

## Author

Computer Science graduate focused on **systems programming, compilers, computer architecture, and computer graphics**.
