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

class LLVMLoadSymbolResolver
{
public:
    virtual llvm::Value *loadSymbolValue(const std::string& symbol) = 0;
protected:
    ~LLVMLoadSymbolResolver() {};
};

class LLVMStoreSymbolResolver
{
public:
    virtual llvm::Value *storeSymbolValue(const std::string& symbol,
            llvm::Value *value) = 0;
protected:
    ~LLVMStoreSymbolResolver() {};
};

} /* namespace rr */
#endif /* LLVMCodeGenH */
