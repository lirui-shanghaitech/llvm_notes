## Lpsolve turtorial

#### Problem formulation

Suppose a farmer has 75 acres on which to plant two crops: wheat and barley. To produce these crops, it costs the farmer (for seed, fertilizer, etc.) $120 per acre for the wheat and $210 per acre for the barley. The farmer has $15000 available for expenses. But after the harvest, the farmer must store the crops while awaiting favourable market conditions. The farmer has storage space for 4000 bushels. Each acre yields an average of 110 bushels of wheat or 30 bushels of barley. If the net profit per bushel of wheat (after all expenses have been subtracted) is $1.30 and for barley is $2.00, how should the farmer plant the 75 acres to maximize profit?

Mathmatically, we can formulate this as :

```
Maximize P = (110)(1.30)x + (30)(2.00)y = 143x + 60y
s.t.            
				120x + 210y <= 15000
                110x + 30y <= 4000
                x + y <= 75
                x >= 0, y >= 0
```

where  `x` denote the number of acres allotted to wheat and `y` the number of acres allotted to barley, the optimal solution of this problem is 

```
x = 21.875
y = 75 - 21.875 = 53.125
```

How to solve this in LPSOLVE ?

#### Construct model from C/C++

1. #####  Setup the problem

```c++
lprec *lp =  make_lp(0, Ncol); // there are two variables in the model, Ncol = 2
set_col_name(lp, 1, "x");	   // name our variables. Not required, but can be useful 
set_col_name(lp, 2, "y");
```

2. ##### Add constraints (examples)

```c++
set_add_rowmode(lp, TRUE); // makes building the model faster if it is done rows by row
REAL row[1+2];     /* must be 1 more than number of columns ! */
REAL sparserow[2]; /* must be the number of non-zero values */
int colno[2];

/* Create a new LP model */
row[1] = 120;
row[2] = 210; /* also zero elements must be provided */
add_constraint(lp, row, LE, 15000); /* constructs the row: 120x + 210y <= 15000 */

colno[0] = 1; sparserow[0] = 110; /* column 1 */
colno[1] = 2; sparserow[1] = 30; /* column 2 */
add_constraintex(lp, 2, sparserow, colno, LE, 4000); /* constructs the row: 110x + 30y <= 4000 */
set_add_rowmode(lp, FALSE);

// You can also set the row name to name the constraints
set_row_name(lp, 1, "row1");
set_row_name(lp, 1, "row2");
```

* Note that for **add_constraint** (and **add_constraintex** when *colno* is NULL) element 0 of the array is not considered (i.e. ignored). Column 1 is element 1, column 2 is element 2, ..., that is why row[1+2] must have `three` elements
* **add_constraintex** has the possibility to specify only the non-zero elements. In that case *colno* specifies the column numbers of the non-zero elements. Both *row* and *colno* are then zero-based arrays. This will speed up building the model considerably if there are a lot of zero values. In most cases the matrix is sparse and has many zero value. Note that **add_constraintex** behaves the same as **add_constraint** when *colno* is NULL.
* LE:  Less than or equal (<=), EQ:  Equal (=), GE:  Greater than or equal (>=)
* Note that: **ROW0** is the objective function, so when index the constraints start from 1, for instance ` set_mat(lp, 1, 1, 3.14);` set the first constaints to be `3.14x+210y <= 15000`, also when you iterate through variables you must start with 1 (strange it not start with zero)

|              | COL0 (Unsed) | COL1 (X) | COL2 (Y) |
| ------------ | ------------ | -------- | -------- |
| ROW0 (Obj)   | X            | 143      | 60       |
| ROW1 (Cons1) | X            | 120      | 210      |
| ROW2 (Cons2) | X            | 110      | 30       |

3. ##### Set objective

There are four functions to set your objective function

```c++
unsigned char set_obj_fn(lprec *lp, REAL *row);
unsigned char set_obj_fnex(lprec *lp, int count, REAL *row, int *colno);
unsigned char str_set_obj_fn(lprec *lp, char *row_string);
unsigned char set_obj(lprec *lp, int column, REAL value);
```

Note that it is advised to set the objective function before adding rows via [add_constraint, add_constraintex, str_add_constraint](http://lpsolve.sourceforge.net/5.5/add_constraint.htm). This especially for larger models. This will be much more performant than adding the objective function afterwards (Here, we did not use it in such way).

```c++
REAL row[1+2]; /* must be 1 more then number of columns ! */
/* Create a new LP model */
row[1] = 143;
row[2] = 60;
set_obj_fn(lp, row);  /* constructs the obj: 143x + 60y */

/* Another way using set_obj_fnex */
REAL sparserow[2]; /* must be the number of non-zero values */
int colno[2];
colno[0] = 1; sparserow[0] = 143; /* column 1 */
colno[1] = 2; sparserow[1] = 60; /* column 2 */
set_obj_fnex(lp, 2, sparserow, colno);
```

* Note that for **set_obj_fn** (and **set_obj_fnex** when *colno* is NULL) element 0 of the array is not considered (i.e. ignored). Column 1 is element 1, column 2 is element 2, ...
* **set_obj_fnex** has the possibility to specify only the non-zero elements. In that case *colno* specifies the column numbers of the non-zero elements. This will speed up building the model considerably if there are a lot of zero values. In most cases the matrix is sparse and has many zero value. Thus it is almost always better to use **set_obj_fnex** instead of **set_obj_fn**. **set_obj_fnex** is always at least as performant as **set_obj_fn**. Note that unspecified values by **set_obj_fnex** are set to zero.
* The **set_obj** function sets the objective value for the specified column. If multiple objective values must be set, it is more performant to use **set_obj_fnex**.

4. ##### Solve problem

First you need to tell lpsolve whether you want to maximize or minimize ojective function, the call `solve ` function to solve it

```c++
set_maxim(lp);
/* Just out of curioucity, now show the model in lp format on screen */
write_LP(lp, stdout); 
/* I only want to see important messages on screen while solving */
set_verbose(lp, IMPORTANT); 
/* Now let lpsolve calculate a solution */
int ret = solve(lp);
```

The solve will return integer values to indicate the state of solving process `OPTIMAL `, `SUBOPTIMAL `, `INFEASIBLE ` ..., you can use these values to judge whether lpsolve successfully solve the problem.

5. ##### Get the result

```c++
/* Returns the value of the objective function. */
REAL get_objective(lprec *lp);
printf("Objective value: %f\n", get_objective(lp));

/* There are two ways to get final variable value */
REAL var[2], *ptr_var;
get_variables(lp, var);
get_ptr_variables(lp, &ptr_var);
```

* The **get_variables**, **get_ptr_variables** functions retrieve the values of the variables. These values are only valid after a successful [solve](http://lpsolve.sourceforge.net/5.5/solve.htm) or [lag_solve](http://lpsolve.sourceforge.net/5.5/lag_solve.htm). Function **get_variables** needs an array that is already dimensioned with [get_Ncolumns](http://lpsolve.sourceforge.net/5.5/get_Ncolumns.htm) elements. **get_ptr_variables** returns a pointer to an array already dimensioned by lp_solve. Element 0 will contain the value of the first variable, element 1 of the second variable, ...



**!!! The offical [lp_solve_API_reference](http://lpsolve.sourceforge.net/5.5/) has may examples, after this, you should look those examples to further familiar with lp_solve.**  The code of this turtorial can be found [here](http://lpsolve.sourceforge.net/5.5/).