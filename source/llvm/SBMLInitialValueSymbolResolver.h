/*
 * SBMLInitialValueSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef SBMLInitialValueSymbolResolver_H_
#define SBMLInitialValueSymbolResolver_H_

#include "LoadSymbolResolverBase.h"
#include "LLVMIncludes.h"
#include "LLVMModelDataSymbols.h"
#include "LLVMModelSymbols.h"

namespace libsbml
{
class Model;
}

namespace rrllvm
{

/**
 * pulls values from the original sbml document.
 */
class SBMLInitialValueSymbolResolver: public LoadSymbolResolverBase
{
public:
    SBMLInitialValueSymbolResolver(const libsbml::Model *model,
            const LLVMModelDataSymbols &modelDataSymbols,
            const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder);

    virtual ~SBMLInitialValueSymbolResolver() {};

    virtual llvm::Value *loadSymbolValue(const std::string& symbol,
            const llvm::ArrayRef<llvm::Value*>& args =
                    llvm::ArrayRef<llvm::Value*>());

};

} /* namespace rr */
#endif /* SBMLInitialValueSymbolResolver_H_ */
