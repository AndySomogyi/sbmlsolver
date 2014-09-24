/*
 * DistribFunctionResolver.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: andy
 */

#include <llvm/DistribFunctionResolver.h>

#ifdef LIBSBML_HAS_PACKAGE_DISTRIB

#include "LLVMException.h"
#include "rrLogger.h"

using namespace llvm;
using namespace rr;

namespace rrllvm
{

DistribFunctionResolver::DistribFunctionResolver(
        const ModelGeneratorContext& ctx, llvm::Value *modelData) :
                builder(ctx.getBuilder()),
                random(ctx.getRandom()),
                modelData(modelData),
                modelDataSymbols(ctx.getModelDataSymbols()),
                module(ctx.getModule())
{
}


llvm::Value* DistribFunctionResolver::loadSymbolValue(
        const libsbml::FunctionDefinition* funcDef,
        const libsbml::DistribFunctionDefinitionPlugin* distribFunc,
        const llvm::ArrayRef<llvm::Value*>& args)
{
    Log(Logger::LOG_NOTICE) << "distrib function: " << funcDef->getId();

    ModelDataIRBuilder mdbuilder(modelData, modelDataSymbols,
                builder);

    // pointer to random field.
    llvm::Value *randomPtr = mdbuilder.createRandomLoad();

    if (args.size() != 2)
    {
        throw_llvm_exception("invalid number of args");
    }

    llvm::Value *funcArgs[] = {randomPtr, args[0], args[1]};

    llvm::Value *func = module->getFunction("rr_distrib_uniform");

    return builder.CreateCall(func, funcArgs, "call_rr_distrib_uniform");
}




} /* namespace rrllvm */

#endif /* LIBSBML_HAS_PACKAGE_DISTRIB */

