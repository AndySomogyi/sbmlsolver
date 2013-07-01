#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMModelDataSymbols.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace rr;
using namespace llvm;



int main(int argc, char* argv[])
{
    cout << "hi\n";
    LLVMModelGeneratorContext c;

    StructType *s = LLVMModelDataValue::getStructType(c.getModule(), c.getExecutionEngine());


    return 0;
}

