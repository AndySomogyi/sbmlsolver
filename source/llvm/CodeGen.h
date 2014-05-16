/*
 * CodeGen.h
 *
 *  Created on: Jul 2, 2013
 *      Author: andy
 */

#ifndef CodeGenH
#define CodeGenH

#include "LLVMIncludes.h"
#include <stack>
#include <string>

namespace rrllvm
{

class CodeGen
{
public:
    virtual llvm::Value *codeGen() = 0;
protected:
    ~CodeGen() {};
};

/**
 * special name for the time symbol
 */
#define SBML_TIME_SYMBOL "\\time"

class LoadSymbolResolver
{
public:
    /**
     * generate an LLVM load instruction.
     *
     * @param symbol: the symbol name to resolve
     * @param args: function argument list
     */
    virtual llvm::Value *loadSymbolValue(const std::string& symbol,
            const llvm::ArrayRef<llvm::Value*>& args =
                    llvm::ArrayRef<llvm::Value*>()) = 0;

    virtual void recursiveSymbolPush(const std::string& symbol) = 0;

    virtual void recursiveSymbolPop() = 0;

protected:

    virtual ~LoadSymbolResolver() {};
};

class StoreSymbolResolver
{
public:
    virtual llvm::Value *storeSymbolValue(const std::string& symbol,
            llvm::Value *value) = 0;

protected:
    ~StoreSymbolResolver() {};
};



} /* namespace rr */
#endif /* LLVMCodeGenH */
