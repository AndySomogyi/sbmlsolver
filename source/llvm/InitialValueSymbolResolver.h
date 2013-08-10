/*
 * InitialValueSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef InitialValueSymbolResolver_H_
#define InitialValueSymbolResolver_H_

#include "CodeGen.h"
#include "LLVMIncludes.h"
#include "LLVMModelDataSymbols.h"
#include "LLVMModelSymbols.h"

namespace libsbml
{
class Model;
}

namespace rr
{

class InitialValueSymbolResolver: public LoadSymbolResolver
{
public:
    InitialValueSymbolResolver(const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder);

    virtual ~InitialValueSymbolResolver();

    virtual llvm::Value* loadSymbolValue(const std::string& symbol);

protected:
    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    const LLVMModelSymbols &modelSymbols;
    llvm::IRBuilder<> &builder;

};

} /* namespace rr */
#endif /* InitialValueSymbolResolver_H_ */
