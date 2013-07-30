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
#include "rrLLVMBasicSymbolResolver.h"

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
class LLVMInitialValueTermSymbolResolver: public LLVMSymbolResolver
{
public:
    LLVMInitialValueTermSymbolResolver(const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder,
            LLVMSymbolResolver &parent);

    virtual ~LLVMInitialValueTermSymbolResolver();


    /**
     * The runtime resolution of symbols first search through the
     * replacement rules, applies them, them pulls the terminal
     * symbol values from the ModelData struct.
     *
     * The initial assigment generator overrides this and pulls
     * the terminal values from the initial values and assigments
     * specified in the model.
     */
    virtual llvm::Value *loadSymbolValue(const std::string& symbol);

    virtual llvm::Value *storeSymbolValue(const std::string& symbol);

protected:

    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    const LLVMModelSymbols &modelSymbols;
    llvm::IRBuilder<> &builder;
    LLVMSymbolResolver &parent;
};

class LLVMInitialValueSymbolResolver: public LLVMBasicSymbolResolver
{
public:
    LLVMInitialValueSymbolResolver(const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder);

    virtual ~LLVMInitialValueSymbolResolver();

protected:
    LLVMInitialValueTermSymbolResolver terminal;

};

} /* namespace rr */
#endif /* rrLLVMInitialValueSymbolResolver_H_ */
