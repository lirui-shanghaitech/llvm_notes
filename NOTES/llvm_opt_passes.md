### LLVM Analysis and Transfome Passes

There are three types of LLVM passes

* **Analysis passes** compute information that other passes can use or for debugging or program visualization purposes.
* **Transform passes** can use (or invalidate) the analysis passes. Transform passes all mutate the program in some way. 
* **Utility passes** provides some utility but don’t otherwise fit categorization. For example passes to extract functions to bitcode or write a module to bitcode are neither analysis nor transform passes. 

#### Importrant Analysis Passes

TODO



#### Important Transform Passes

##### Loop simplify pass

This pass performs several transformations to transform natural loops into a simpler form, which makes subsequent analyses and transformations simpler and more effective. This pass obviously modifies the CFG, but updates loop information and dominator information.