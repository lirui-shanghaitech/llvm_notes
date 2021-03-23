#ifndef _FIND_FUNCTIONS
#define _FIND_FUNCTIONS
// related headers should be included.
#include "tprint.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
// #include "llvm/PassAnalysisSupport.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/LoopVersioning.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include <array>
#include <bits/stl_map.h>
#include <cstdio>
#include <cxxabi.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

using namespace llvm;

class find_functions : public ModulePass
{
    public:
        // Initialization function, you should add own initialization here
        find_functions() : ModulePass(ID) {
            // Make a file to record the function names
            function_names = new raw_fd_ostream("function_names", ErrInfo, sys::fs::F_None);
        }

        ~find_functions() {
            function_names->flush();
            delete function_names;
        }

        // A virtual function in Module class, all inherit classes should make their own definition
        bool runOnModule(llvm::Module &M);
        // the ID for pass should be initialized but the value does not matter, since LLVM uses the
        // address of this variable as label instead of its value.
        static char ID;
        
        
        // Define your auxillary functions here
        std::error_code ErrInfo;
        raw_ostream *function_names;
};

#endif
