# cosh

An interactive math shell.

## Features

### Numbers

This project supports rational numbers with arbitrary precision. You can use `.` or `,` to input decimals.

### Variables

You can declare variables with `=` to reuse values later. The variable `Ans` always contains the last result.

### Operators

These operators are supported:

- `x + y`: Addition
- `x - y`: Subtraction
- `x * y`: Multiplication
- `x / y`: Division
- `x % y`: Remainder
- `x ^ y`: Exponentiation
- `(...)`: Grouping of operators
- `|...|`: Grouping with absolute value

### Functions

These functions are supported:

- `abs x`: Absolute value
- `ceil x`: Ceiling
- `den x`: Denominator
- `floor x`: Floor
- `inv x`: Inverse
- `neg x`: Negation
- `num x`: Numerator
- `sgn x`: Sign
- `trunc x`: Truncation

## Installation

Make sure you have GCC, GMP and make installed. On Debian-based systems, you can install them with:

```
apt install build-essential libgmp-dev
```

After that, clone the repository, change the directory and build the project:

```
git clone https://github.com/vaporoxx/cosh
cd cosh
make
```

The executable is now available as `./cosh` and can be moved to a different directory.

## Usage

```
cosh [-d] [-h] [-i INPUT]
```

| Option          | Description                                       |
|-----------------|---------------------------------------------------|
| `-d`, `--debug` | Prints the AST of the input instead of running it |
| `-h`, `--help`  | Prints this help message                          |
| `-i`, `--input` | Runs `INPUT` instead of being interactive         |
