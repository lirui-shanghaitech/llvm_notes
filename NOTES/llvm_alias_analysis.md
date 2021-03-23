### LLVM Alias / Data dependence analysis

Memory alias analysis

* **Assumption**: no dynamic memory, pointers can point only to variables

* **Goal**: at each program point, compute set of (p->x) pairs if p points to variable x
* **Approach**: 
    * Based on data-flow analysis 
    * May information