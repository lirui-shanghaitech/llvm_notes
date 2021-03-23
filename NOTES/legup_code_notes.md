### LegUp code notes

#### Allocation



#### Scheduling



#### Loop Scheduling



#### Binding



#### Dynamic Scheduling



#### Interface Synthesis



#### Verilog Constructor

Construct RTL from scratch

##### RTL Module

**`RTLWidth`** : Represents the bitwidth of a `RTLSignal` i.e. [5:2]

**`RTLSignal`** : Represents an RTL always block for a wire/reg that can be assigned to other RTLSignals each predicated on an RTLSignal to form a mux.  RTL wire/reg always block

**`RTLOp`** : An operation with one, two, or three operands. Each operand is a RTLSignal. This class inherit from RTLSignal.

**`RTLConst`** : Constant operand, it inherits from RTLSignal.

**`RTLModule`** : Represents an RTL module. Contains lists of:

* RTLSignal signals

* RTLSignal ports

* RTLSignal parameters

* Instantiated RTLModules

It uses data structure presented in this [DAC paper](https://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=14827), read it for more information.