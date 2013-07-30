/*
 * rrLLVMModelDataSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef rrLLVMModelDataSymbolResolver_H_
#define rrLLVMModelDataSymbolResolver_H_

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
 * A terminal symbol resolver, which resolved everything
 * to values stored in the model data structure.
 *
 * terminal symbol resolvers treat all species as amounts.
 */
class LLVMModelDataTermSymbolResolver: public LLVMSymbolResolver
{
public:
    LLVMModelDataTermSymbolResolver(llvm::Value *modelData,
            const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder);

    virtual ~LLVMModelDataTermSymbolResolver();


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

    llvm::Value *modelData;
    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    llvm::IRBuilder<> &builder;
};

class LLVMModelDataSymbolResolver: public
    LLVMBasicSymbolResolver
{
public:
    LLVMModelDataSymbolResolver(llvm::Value *modelData,
            const libsbml::Model *model,
            const LLVMModelSymbols &modelSymbols,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder);
private:
    LLVMModelDataTermSymbolResolver terminal;
};

} /* namespace rr */
#endif /* rrLLVMModelDataSymbolResolver_H_ */
