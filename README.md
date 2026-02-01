# `cscript`
My implementation of [Robert Nystrom's](https://stuffwithstuff.com/) [jlox](https://craftinginterpreters.com/) interpreter, ported to C++. The code is based on the book [Crafting Interpreters](https://www.craftinginterpreters.com).

## About
`cscript` is a high-level, dynamic, interpreted and weakly-typed object-oriented scripting language. Use the default file extension `.cscpt` (although any file type can theoretically be used).

## Usage

### Requirements
- clang (C++ 20 or later)
- Unix-based OS (Windows is not supported)

### Run
To get started, clone this repository and run:
- `make all` to get a build directory and the executable
- `make clean` to remove the build directory
- `make clear` to remove the build directory and the executable
- `./cscpt.out` with no arguments to enter the REPL, or a `cscript` file (`.cscpt`) as an argument to run it


## Language Features

### Hello, World!

```c
ioputfn "Hello, World!"; // comment
```

`ioputfn` is a native function that prints the parameters with a newline, built-in to `cscript`. It takes any primitive data type evaluating to an r-value as input. To print without a newline, use `ioputf`.

Statements are terminated with a semicolon.


---
### Variables & Data Types

`Cscript` is dynamic and weakly-typed, i.e., type checking is done at runtime. There are three internally defined data types derived from C++: `double`, `bool` (with values `true` and `false`) and `string` (enclosed in `"..."`) and two fallback values for a variable: `nil` and `uninitialised_t`.

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
### Scope & Resolution

A block of statements is enclosed in braces `{}`. Variables declared inside the block shadow the enclosing scope and their lifetime is limited to within the block. However, if a local variable identifier conflicts with a variable in a higher scope, it throws an error during a static pass. This happens because the interpreter creates a new scope for a local variable as soon as it encounters `var`.

The following is illegal:
```cs
var a := "global";
{
    var a := a; // cannot assign before initialistion
}
```


---
### Conditionals

`if` and `else` statements are used, but the boolean condition need not be parenthesised. The block of statements following the `if` or `else` header may be parenthesised and only the first statement will be executed if not.

```hs
if x < y then do ioputfn x;

else do {
    
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

Functions are first-class members. They can be passed as r-values to variables and called.

```js
def fun() as do { ioputfn "foo"; }
def fn() as do { fun(); }

var f := fn;
f(); // prints 'foo'
```

---
### Classes

`cscript` supports object-oriented programming. It 'classes' an entity 'as' a collection of methods. Data members can be added dynamically on the go.

```py
class Foo as {

    fun() as do { ...; }
    foo() as do { ...; }
}
```

Data members can also be initialised using a non-overloadable constructor with the keyword `init`. Class attributes can be referenced using `this`.

```js
class Foo as {

    init(a,b) as do {

        this.a := a; // initialised using
        this.b := b; // the constructor
        this.c := nil; // created on the go
    }

    fn() as do { ioputfn "test" + this.b; } // access a data member
    fun() as do { this.fn(); } // call a method
}
```

Objects of a class can be instantiated as a normal variable using `var`.

```js
var foo := Foo("p1", "p2");
foo.fun(); // prints 'testp2'
```

All class methods are static, i.e., they do not require any explicit instance to run upon. They can be called by constructing an object on the fly. This creates a temporary instance in the scope which dies as soon as it executes.

```js
Foo("foo", "bar").fn(); // prints 'testfoo'
```

You can check the type of of an object, class or method by just printing it.

```js
ioputfn Foo; // <class Foo>
ioputfn foo; // <instance <class Foo>>
ioputfn foo.fun; // <fn fun>
```

You can also print `this` inside a class to obtain information about it.

```cs
class Bar as {

    init() as do { ioputfn this; }
}

Bar(); // prints '<instance <class Bar>>'
```

---
### Inheritance

`cscript` has support for single inheritance. Use the `>` operator to denote the base class. All functions and variables of a base class are inherited by the subclass.

```js
class A as { // base

    init(a) as do { this.a := a; }

    foo() as do { ioputfn this.a; }
    bar() as do { ioputfn "base"; }
}

class B > A as { // derived

    init(a,b) as do {

        super.init(a); // call base class constructor
        this.b := b;
    }

    bar() as do { ioputfn "derived"; } // override base class method
}

var b := B(1,2);
ioputfn b.a + " " + b.b; // prints '1 2' ; 'a' is inherited
```

All methods of a base class are virtual and will be overridden if possible in the derived class.

```js
b.foo(); // prints '1' ; inherited method
A(nil).bar(); // prints 'base' ; base method
b.bar(); // prints 'derived' ; overriden method
```

Base class methods can be called using `super`.

```js
class B > A as {

        ...

    bar() as do {

        super.bar();
        ioputfn "derived";
    }
}

B(1,2).bar(); // prints 'base\nderived'
```
