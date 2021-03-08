#include "find_functions.h"
#include "tprint.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <ios>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

bool find_functions::runOnModule(Module& M) {
    // Module iterator is used to iterate the functions in the module, ++GI is better
    // than GI ++, for the sake of stability
    for (Module::iterator GI = M.begin(), GE = M.end(); GI != GE; ++GI) {
        *function_names << "Found function: " << GI->getName().str() << "\n";
    }

    function_names->flush();
    return false;
}

char find_functions::ID = 0;