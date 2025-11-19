# cscript
My implementation of Robert Nystrom's jlox interpreter, ported to C++.

## About
cscript is a high-level, dynamic, interpreted and weakly-typed scripting language.

## Usage

### Requirements
- clang (C++ 20 or later)
- Unix-based OS (Windows is not supported)

### Run
To get started, clone this repository and run:
- `make all` to get a build directory and the executable
- `make clean` to remove the build directory
- `make clear` to remove the build directory and the executable
- `./cscpt.out` with no arguments to enter the REPL, or a cscript file (`.cscpt`) as an argument to run it


## Language Features

### Hello, World!

```c
ioputfn "Hello, World!"; // comment
```

`ioputfn` is a native function that prints the parameters with a newline, built-in to cscript. It takes any primitive data type evaluating to an r-value as input. To print without a newline, use `ioputf`.

Statements are terminated with a semicolon.


---
### Variables & Data Types

Cscript is dynamic and weakly-typed, i.e., type checking is done at runtime. There are three internally defined data types derived from C++: `double`, `bool` (with values `true` and `false`) and `string` (enclosed in `"..."`) and two fallback values for a variable: `nil` and `uninitialised_t`.

Use the `var` keyword to declare a variable. Use the assignment operator `:=` to initialise. Trying to access an unitialised variable will cause a runtime error.

```js
var a := 1;

var b; // set to uninitialised_t
ioputfn b; // ERROR

/*
    /*
        nested multiline comments
    */
*/

var c;
c := nil;
ioputfn c; // this is fine
```

Types can be mutated at runtime.

```js
var a := nil; // good practice in place of uninitialised variables
a := true; // mutated to bool
ioputfn a + " is truth"; // automatically mutated to string
```


---
### Operators & Expressions

| Operator | Description |
| :---: | :---: |
| `+` | Addition/Concatenation |
| `-` | Subtraction/Unary negative |
| `*` | Multiplication |
| `/` | Division |
| `^` | Exponentation |
| `%` | Floating-point remainder |
| `!` | Logical negation |
| `=` | Equality |
| `/=` | Non-equality |
| `and` | Logical AND |
| `or` | Logical OR |

The equality operators can be used to compare at the data type level as well.

Expressions can be parenthesised and precedence works in the expected way, carried over from C.


---
### Scope

A block of statements is enclosed in braces `{}`. Variables declared inside the block shadow the enclosing scope and their lifetime is limited to within the block.


---
### Conditionals

`if` and `else` statements are used, but the boolean condition need not be parenthesised. The block of statements following the `if` or `else` header may be parenthesised and only the first statement will be executed if not.

```hs
if x < y then do ioputfn x;

else {
    
    if x = y then do ioputfn y;

    else do ioputfn false;
}
```


---
### Loops

`for` and `while` loops work as expected and no parenthesis are needed around the loop conditions.

```js
for var i := 0; i < 10; i := i + 1; do
    ioputfn "Hello, World";

var i := 0;
while i < 10 do {

    ioputfn i;
    i := i + 1;
}
```


---
### Functions

Functions must be declared before they are used. Use the `def` keyword to start a function declaration and parenthesise the arguments in a comma-separated list. Start the function body block with the keywords `as do`. Use the `jump` keyword to return a value from a non-void function.

```py
def fib(n) as do {

    if n = 0 then do jump 0;
    else do if n = 1 then do jump 1;

    else do jump fib(n - 1) + fib(n - 2);
}
```

Function calls can be made by using the identifier followed by `()`.
```c
ioputfn fib(5);
```