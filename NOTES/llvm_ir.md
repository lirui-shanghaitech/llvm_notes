### LLVM IR notes

#### Basic constructs

##### Global variable

```c++
int variable = 21;
@variable = global i32 21
```

Globals are prefixed with the `@` character. You can see that also functions, such as `main`, are also global variables in LLVM. **LLVM views global variables as pointers;**  so you must explicitly dereference the global variable using the `load` instruction when accessing its value, likewise you must explicitly store the value of a global variable using the `store` instruction.

##### Local variables

- Temporary variables/Registers: created by introducing a new symbol for the variable
- Stack-allocated local variables: created by allocating the variable on the stack

```assembly
%reg = add i32 4, 2
%stack = alloca i32
```

Notice that `alloca` yields a pointer to the allocated type. As is generally the case in LLVM, you must explicitly use a `load` or `store` instruction to read or write the value respectively.

##### Constants

- Constants that do *not* occupy allocated memory.
- Constants that *do* occupy allocated memory.

```
%1 = add i32 %0, 17     ; 17 is an inlined constant

@hello = internal constant [6 x i8] c"hello\00"
%struct = type { i32, i8 }
@struct_constant = internal constant %struct { i32 16, i8 4 }
```

Such a constant is really a global variable whose visibility can be limited with `private` or `internal` so that it is invisible outside the current module.

##### Structures

```
struct Foo { int a; char *b; double c; };
%Foo = type {
    i32,        ; 0: a
    i8*,        ; 1: b
    double      ; 2: c
}
//---------------------------------------------------------------------------------------
Foo foo;
char **bptr = &foo.b;

%foo = alloca %Foo
// char **bptr = &foo.b GetElementPointer GEP instruction
%1 = getelementptr %Foo, %Foo* %foo, i32 0, i32 1
//---------------------------------------------------------------------------------------
Foo bar[100];
bar[17].c = 0.0;

; Foo bar[100]
%bar = alloca %Foo, i32 100
; bar[17].c = 0.0
%2 = getelementptr %Foo, %Foo* %bar, i32 17, i32 2
store double 0.0, double* %2
```

##### Casts

- **Bitwise casts (type casts)**: A bitwise cast (`bitcast`) reinterprets a given bit pattern without changing any bits in the operand. 

```
Foo *foo = (Foo *) malloc(sizeof(Foo));

%1 = call i8* @malloc(i32 4)
%foo = bitcast i8* %1 to %Foo*
```

- **Zero-extending casts (unsigned upcasts)**: To upcast an unsigned value like in the example below using ```zext```

```
uint8 byte = 117;
uint32 word;
word = byte;

@byte = global i8 117
@word = global i32 0
%1 = load i8, i8* @byte
%2 = zext i8 %1 to i32
store i32 %2, i32* @word
```

- **Sign-extending casts (signed upcasts)**: To upcast a signed value, you replace the `zext` instruction with the `sext` instruction 

```
@char = global i8 -17
@int  = global i32 0
%1 = load i8, i8* @char
%2 = sext i8 %1 to i32
store i32 %2, i32* @int
```

- **Truncating casts (signed and unsigned downcasts)**: Both signed and unsigned integers use the same instruction, `trunc`, to reduce the size of the number in question. 

```
@int = global i32 -1
@char = global i8 0
%1 = load i32, i32* @int
%2 = trunc i32 %1 to i8
store i8 %2, i8* @char
```

- **Floating-point extending casts (float upcasts)**: Floating points numbers can be extended using the `fpext` instruction

```
@small = global float 1.25
@large = global double 0.0
%1 = load float, float* @small
%2 = fpext float %1 to double
store double %2, double* @large
```

- **Floating-point truncating casts (float downcasts)**: Likewise, a floating point number can be truncated to a smaller size using ```fptrunc```

```
@large = global double 1.25
@small = global float 0.0
%1 = load double, double* @large
%2 = fptrunc double %1 to float
store float %2, float* @small
```

- **Pointer-to-integer casts**: Pointers do not support arithmetic, which is sometimes needed when doing systems programming. LLVM has support for casting pointer types to integer types using the `ptrtoint` instruction.
- **Integer-to-pointer casts**: The `inttoptr` instruction is used to cast an integer back to a pointer
- **Address-space casts (pointer casts)**.



##### Function Definitions and Declarations

* Simple definition and Declaration

```
define  i32 @Bar() nounwind { ret i32 17 }
declare i32 @Bar(i32 %value)
```

* With variable number of parameters

```c++
declare i32 @printf(i8*, ...) nounwind

@.textstr = internal constant [20 x i8] c"Argument count: %d\0A\00"

define i32 @main(i32 %argc, i8** %argv) nounwind {
    ; printf("Argument count: %d\n", argc)
    %1 = call i32 (i8*, ...) @printf(i8* getelementptr([20 x i8], [20 x i8]* @.textstr, i32 0, i32 0), i32 %argc)
    ret i32 0
}
```

* Overloading: function names are mangled in the LLVM IR level

```
define i32 @_Z8functionii(i32 %a, i32 %b) #0 {
; [...]
  ret i32 %5
}

define double @_Z8functionddd(double %a, double %b, double %x) #0 {
; [...]
  ret double %8
}
```

* For the detailed description of ```struct``` arguments in the function please refer to this [link](https://mapping-high-level-constructs-to-llvm-ir.readthedocs.io/en/latest/basic-constructs/functions.html#struct-by-value-as-parameter-or-return-value)

* Function Points

```
int (*Function)(char *buffer);
@Function = global i32(i8*)* null
```

##### Unions

TODO: since it is not commonly used today



#### Control-Flow Constructs

##### ```if-then-else``` conversion

In LLVM IR control-flow is implemented by jumping between *basic blocks*, which contain instruction sequences that do not change control flow. Each basic block ends with an instruction that changes the control flow. The most common branching instruction is[ `br` ](https://mapping-high-level-constructs-to-llvm-ir.readthedocs.io/en/latest/control-structures/if-then-else.html#brlink)

```
// Usage of br instruction
br i1 %cond, label %iftrue, label %iffalse
br label %dest
```

Max function example

```c++
int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}
```

Translate into LLVM IR: there are four basic blocks

```
define i32 @max(i32 %a, i32 %b) {
entry:
  %retval = alloca i32, align 4
  %0 = icmp sgt i32 %a, %b
  br i1 %0, label %btrue, label %bfalse

btrue:                                      ; preds = %2
  store i32 %a, i32* %retval, align 4
  br label %end

bfalse:                                     ; preds = %2
  store i32 %b, i32* %retval, align 4
  br label %end

end:                                     ; preds = %btrue, %bfalse
  %1 = load i32, i32* %retval, align 4
  ret i32 %1
}
```

##### ```PHI```

The `phi` instruction is named after the φ function used in the theory of SSA. This functions magically chooses the right value, depending on the control flow. In LLVM you have to manually specify the name of the value and the previous basic block.

```
%retval = phi i32 [%a, %btrue], [%b, %bfalse]
```

After using the ```PHI``` function the above max function can be transform to

````
define i32 @max(i32 %a, i32 %b) {
entry:
  %0 = icmp sgt i32 %a, %b
  br i1 %0, label %btrue, label %bfalse

btrue:                                      ; preds = %2
  br label %end

bfalse:                                     ; preds = %2
  br label %end

end:                                     ; preds = %btrue, %bfalse
  %retval = phi i32 [%a, %btrue], [%b, %bfalse]
  ret i32 %retval
}
````

Usually the compiler back end will use the stack for implementing the `phi` instruction. However, if we use a little more optimization in the back end (i.e., `llc -O1`), we can get a more optimized version.



#### Object Oriented Constructs

##### Classes

A class is nothing more than a structure with an associated set of functions that take an implicit first parameter, namely a pointer to the structure. Therefore, is is very trivial to map a class to LLVM IR

```c++
#include <stddef.h>
class Foo
{
public:
    Foo() { _length = 0; }

    void SetLength(size_t value) { _length = value; }
private:
    size_t _length;
};
```

first transform this code into two separate pieces:

- The structure definition.
- The list of methods, including the constructor.

```
; The structure definition for class Foo.
%Foo = type { i32 }

; The default constructor for class Foo.
define void @Foo_Create_Default(%Foo* %this) nounwind {
    %1 = getelementptr %Foo, %Foo* %this, i32 0, i32 0
    store i32 0, i32* %1
    ret void
}

; The Foo::SetLength() method.
define void @Foo_SetLength(%Foo* %this, i32 %value) nounwind {
    %1 = getelementptr %Foo, %Foo* %this, i32 0, i32 0
    store i32 %value, i32* %1
    ret void
}
```

Then we make sure that the constructor (`Foo_Create_Default`) is invoked whenever an instance of the structure is created.

##### Virtual Methods

A virtual method is no more than a compiler-controlled function pointer. Each virtual method is recorded in the `vtable`, which is a structure of all the function pointers needed by a given class. Please refer this [link](https://mapping-high-level-constructs-to-llvm-ir.readthedocs.io/en/latest/object-oriented-constructs/index.html) to learn more about Objected-Oriented Constructs.



#### Some important Instructions

##### [```getelementptr``` Ins](https://llvm.org/docs/LangRef.html#id1743), also fantastic notes from official [website](https://llvm.org/docs/GetElementPtr.html)

Syntax

```
<result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
<result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
<result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
```

