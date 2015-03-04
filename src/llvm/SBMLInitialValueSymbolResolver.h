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

    /**
     * need a modelData ptr because it holds state needed from the random
     * number generators, could be used for init assignmnets.
     */
    SBMLInitialValueSymbolResolver(llvm::Value *modelData,
            const ModelGeneratorContext& ctx);


    virtual ~SBMLInitialValueSymbolResolver() {};

    virtual llvm::Value *loadSymbolValue(const std::string& symbol,
            const llvm::ArrayRef<llvm::Value*>& args =
                    llvm::ArrayRef<llvm::Value*>());

private:
    llvm::Value *modelData;

};

} /* namespace rr */
#endif /* SBMLInitialValueSymbolResolver_H_ */
