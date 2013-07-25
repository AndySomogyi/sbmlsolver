/*
 * rrLLVMModelDataSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef RRLLVMMODELDATASYMBOLRESOLVER_H_
#define RRLLVMMODELDATASYMBOLRESOLVER_H_

#include "rrLLVMCodeGen.h"
#include "rrLLVMIncludes.h"

namespace rr
{

class LLVMModelDataSymbolResolver: public rr::LLVMSymbolResolver
{
public:
    LLVMModelDataSymbolResolver(llvm::Value *modelData);
    virtual ~LLVMModelDataSymbolResolver();

    virtual llvm::Value *symbolValue(const std::string& symbol);

private:
    llvm::Value *modelData;
    const LLVMModelDataSymbols &modelDataSymbols;
    llvm::IRBuilder<> &builder;
    LLVMModelDataIRBuilder modelDataBuilder;
};

} /* namespace rr */
#endif /* RRLLVMMODELDATASYMBOLRESOLVER_H_ */
