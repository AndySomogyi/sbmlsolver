/*
 * rrLLVMCodeGen.h
 *
 *  Created on: Jul 2, 2013
 *      Author: andy
 */

#ifndef LLVMCodeGenH
#define LLVMCodeGenH

#include "rrLLVMIncludes.h"

namespace rr
{

class LLVMCodeGen
{
public:
    virtual llvm::Value *codeGen() = 0;
protected:
    ~LLVMCodeGen() {};
};

class LLVMSymbolResolver
{
public:
    virtual llvm::Value *symbolValue(const std::string& symbol) = 0;
protected:
    ~LLVMSymbolResolver() {};
};

} /* namespace rr */
#endif /* LLVMCodeGenH */
