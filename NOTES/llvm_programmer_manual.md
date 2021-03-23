## Important and Useful LLVM APIs

#### ```isa<>, cast<>, dyn_cast<>``` tempaltes

* ```isa<>```: It returns true or false depending on whether a reference or pointer points to an instance of the specified class. 
* ```cast<>```: It converts a pointer or reference from a base class to a derived class, causing an assertion failure if it is not really an instance of the right type. 
* ```dyn_cast<>```: It checks to see if the operand is of the specified type, and if so, returns a pointer to it (this operator does not work with references). If the operand is not of the correct type, a null pointer is returned.
* ```xxx_or_null```: Operator works just like the `cast<>` operator, except that it allows for a null pointer as an argument (which it then propagates).

```C++
// isa<> and cast<>
static bool isLoopInvariant(const Value *V, const Loop *L) {
  if (isa<Constant>(V) || isa<Argument>(V) || isa<GlobalValue>(V))
    return true;

  // Otherwise, it must be an instruction...
  return !L->contains(cast<Instruction>(V)->getParent());
}

// dyn_cast<>
if (auto *AI = dyn_cast<AllocationInst>(Val)) {
  // ...
}
```

#### ```StringRef``` class

The `StringRef` data type represents a reference to a constant string (a character array and a length) and supports the common operations available on `std::string`.



#### Iterating over the  ```Function``` 

##### Iterate over blocks in a function

```c++
// Iterate over blocks in a function
Function &Func = ...
for (BasicBlock &BB : Func)
  // Print out the name of the basic block if it has one, and then the
  // number of instructions that it contains
  errs() << "Basic block (name=" << BB.getName() << ") has "
             << BB.size() << " instructions.\n";
```

##### Iterate over instructions in a block

```c++
// Iterate over instructions in a block
BasicBlock& BB = ...
for (Instruction &I : BB)
   // The next statement works since operator<<(ostream&,...)
   // is overloaded for Instruction&
   errs() << I << "\n";
```

##### Elegent way of iterating ins in a function

```c++
// Elegent way of iterating ins in a function
#include "llvm/IR/InstIterator.h"
// F is a pointer to a Function instance
for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
  errs() << *I << "\n";
```

##### Iterater over instructions in a function

```c++
// Iterater over instructions in a function
Function* targetFunc = ...;
class OurFunctionPass : public FunctionPass {
  public:
    OurFunctionPass(): callCounter(0) { }

    virtual runOnFunction(Function& F) {
      for (BasicBlock &B : F) {
        for (Instruction &I: B) {
          if (auto *CB = dyn_cast<CallBase>(&I)) {
            // We know we've encountered some kind of call instruction (call,
            // invoke, or callbr), so we need to determine if it's a call to
            // the function pointed to by m_func or not.
            if (CB->getCalledFunction() == targetFunc)
              ++callCounter;
          }
        }
      }
    }

  private:
    unsigned callCounter;
};
```

##### Iterate over the successors and predecessors

```c++
// Iterate over the successors and predecessors
#include "llvm/IR/CFG.h"
BasicBlock *BB = ...;

for (BasicBlock *Pred : predecessors(BB)) {
  // ...
}
```

##### Iterate over def-use

```c++
// Iterate over def-use: we want to determine which Users use the Value
Function *F = ...;

for (User *U : F->users()) {
  if (Instruction *Inst = dyn_cast<Instruction>(U)) {
    errs() << "F is used in instruction:\n";
    errs() << *Inst << "\n";
  }
```

##### Iterate over use-def

```c++
// Iterate over use-def: need to know what Values are used by it
Instruction *pi = ...;

for (Use &U : pi->operands()) {
  Value *v = U.get();
  // ...
}
```

#### Core LLVM Class Hierarchy

**```Module```**: The `Module` class represents the top level structure present in LLVM programs. An LLVM module is effectively either *a translation unit of the original program or a combination of several translation units merged by the linker*. The `Module` class keeps track of a list of [Function](https://llvm.org/docs/ProgrammersManual.html#c-function)s, a list of [GlobalVariable](https://llvm.org/docs/ProgrammersManual.html#globalvariable)s, and a [SymbolTable](https://llvm.org/docs/ProgrammersManual.html#symboltable). 

**```Value```**: The `Value` class is the most important class in the LLVM Source base. It represents a typed value that may be used (among other things) as an operand to an instruction. There are many different types of `Value`s, such as [Constant](https://llvm.org/docs/ProgrammersManual.html#constant)s, [Argument](https://llvm.org/docs/ProgrammersManual.html#argument)s. Even [Instruction](https://llvm.org/docs/ProgrammersManual.html#instruction)s and [Function](https://llvm.org/docs/ProgrammersManual.html#c-function)s are `Value`s. The `Value` class keeps a list of all of the `User`s that is using it, this use list is how LLVM represents def-use information in the program, and is accessible through the `use_*` methods.

*One important aspect of LLVM is that there is no distinction between an SSA variable and the operation that produces it.* Because of this, any reference to the value produced by an instruction (or the value available as an incoming argument, for example) is represented as a direct pointer to the instance of the class that represents this value.

**```User```**: The `User` class is the common base class of all LLVM nodes that may refer to `Value`s. It exposes a list of “Operands” that are all of the `Value`s that the User is referring to. The `User` class itself is a subclass of `Value`.

**```Instruction```** : Because the `Instruction` class subclasses the [User](https://llvm.org/docs/ProgrammersManual.html#user) class, its operands can be accessed in the same way as for other `User`s (with the `getOperand()`/`getNumOperands()` and `op_begin()`/`op_end()` methods).

**```Constant ```**: Constant represents a base class for different types of constants. It is subclassed by ConstantInt, ConstantArray, etc. for representing the various types of Constants. [GlobalValue](https://llvm.org/docs/ProgrammersManual.html#globalvalue) is also a subclass, which represents the address of a global variable or function.

**```Function```** : The `Function` class represents a single procedure in LLVM. It is actually one of the more complex classes in the LLVM hierarchy because it must keep track of a large amount of data. The `Function` class keeps track of a list of [BasicBlock](https://llvm.org/docs/ProgrammersManual.html#basicblock)s, a list of formal [Argument](https://llvm.org/docs/ProgrammersManual.html#argument)s, and a [SymbolTable](https://llvm.org/docs/ProgrammersManual.html#symboltable).

**```BasicBlock```** : This class represents a single entry single exit section of the code, commonly known as a basic block by the compiler community. The `BasicBlock` class maintains a list of [Instruction](https://llvm.org/docs/ProgrammersManual.html#instruction)s, which form the body of the block. Matching the language definition, the last element of this list of instructions is always a terminator instruction.



For more informations, please refer to official [programmer manual](https://llvm.org/docs/ProgrammersManual.html)