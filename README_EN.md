[[日本語]](README.md) [[English]](README_EN.md)

# Metro: v0.0.1
A statically typed scripting language. Powerful and accurate type inference is its key feature.

# Usage
## Clone this repository, and build
```
git clone --recursive git@github.com:bomkei/metro
cd metro
make -j
```

## Run
```
./metro.out [your script files]
```

# How to contribute
Welcome! Read [this](CONTRIBUTE.md).

# Implimentations
## Interpreter

No other libraries or tools such as yacc or lex are used.<br>
The interpreter is written in C++ including lexer and evaluator.

# Memory safety
The garbage collector will manage the objects for you.
