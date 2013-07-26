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

/**
 * A terminal symbol resolver, everything must resolve to
 * an initial value specified in the model
 *
 * terminal symbol resolvers treat all species as amounts.
 */
class LLVMInitialValueSymbolResolver: public LLVMSymbolResolver
{
public:
    LLVMInitialValueSymbolResolver(const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder);

    virtual ~LLVMInitialValueSymbolResolver();


    /**
     * The runtime resolution of symbols first search through the
     * replacement rules, applies them, them pulls the terminal
     * symbol values from the ModelData struct.
     *
     * The initial assigment generator overrides this and pulls
     * the terminal values from the initial values and assigments
     * specified in the model.
     */
    virtual llvm::Value *symbolValue(const std::string& symbol);

protected:

    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    const LLVMModelSymbols &modelSymbols;
    llvm::IRBuilder<> &builder;
};

} /* namespace rr */
#endif /* rrLLVMInitialValueSymbolResolver_H_ */
