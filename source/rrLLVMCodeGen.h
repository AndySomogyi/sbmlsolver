/*
 * rrLLVMCodeGen.h
 *
 *  Created on: Jul 2, 2013
 *      Author: andy
 */

#ifndef LLVMCodeGenH
#define LLVMCodeGenH

#include "rrLLVMIncludes.h"

class LLVMCodeGen
{
public:
    virtual llvm::Value *codeGen() = 0;
protected:
    ~LLVMCodeGen() {};
};




#endif /* LLVMCodeGenH */
