/*
 * rrLLVMInitialValueSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef rrLLVMInitialValueSymbolResolver_H_
#define rrLLVMInitialValueSymbolResolver_H_

#include "rrLLVMCodeGen.h"
#include "rrLLVMIncludes.h"
#include "rrLLVMModelDataSymbols.h"
#include "rrLLVMModelSymbols.h"

namespace libsbml
{
class Model;
}

namespace rr
{

class LLVMInitialValueSymbolResolver: public LLVMLoadSymbolResolver
{
public:
    LLVMInitialValueSymbolResolver(const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder);

    virtual ~LLVMInitialValueSymbolResolver();

    virtual llvm::Value* loadSymbolValue(const std::string& symbol);

protected:
    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    const LLVMModelSymbols &modelSymbols;
    llvm::IRBuilder<> &builder;

};

} /* namespace rr */
#endif /* rrLLVMInitialValueSymbolResolver_H_ */
