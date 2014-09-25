/*
 * DistribFunctionResolver.h
 *
 *  Created on: Sep 23, 2014
 *      Author: andy
 */

#ifndef DISTRIBFUNCTIONRESOLVER_H_
#define DISTRIBFUNCTIONRESOLVER_H_

#include "ModelGeneratorContext.h"
#include <sbml/common/libsbml-config-packages.h>

#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
#include "LLVMIncludes.h"
#include <sbml/packages/distrib/common/DistribExtensionTypes.h>


namespace rrllvm
{

class DistribFunctionResolver
{
public:
    DistribFunctionResolver(const ModelGeneratorContext& ctx,
            llvm::Value *modelData);

    llvm::Value *loadSymbolValue(const libsbml::FunctionDefinition *funcDef,
            const libsbml::DistribFunctionDefinitionPlugin *distribFunc,
            const llvm::ArrayRef<llvm::Value*>& args =
                    llvm::ArrayRef<llvm::Value*>());

private:
    llvm::IRBuilder<> &builder;
    Random *random;
    llvm::Value* modelData;
    const LLVMModelDataSymbols &modelDataSymbols;
    llvm::Module *module;
};

} /* namespace rrllvm */


#endif /* LIBSBML_HAS_PACKAGE_DISTRIB */

#endif /* DISTRIBFUNCTIONRESOLVER_H_ */
