/*
 * rrLLVMBasicSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef rrLLVMBasicSymbolResolver_H_
#define rrLLVMBasicSymbolResolver_H_

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

class LLVMBasicSymbolResolver: public LLVMSymbolResolver
{
public:
    LLVMBasicSymbolResolver(const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder,
            LLVMSymbolResolver *tail = 0);

    virtual ~LLVMBasicSymbolResolver();


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

private:

    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    const LLVMModelSymbols &modelSymbols;
    llvm::IRBuilder<> &builder;
    LLVMSymbolResolver *tail;
};

} /* namespace rr */
#endif /* rrLLVMBasicSymbolResolver_H_ */
