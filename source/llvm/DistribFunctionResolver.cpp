/*
 * DistribFunctionResolver.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: andy
 */

#include <llvm/DistribFunctionResolver.h>

#ifdef LIBSBML_HAS_PACKAGE_DISTRIB


#include "rrLogger.h"

using namespace llvm;
using namespace rr;

namespace rrllvm
{

DistribFunctionResolver::DistribFunctionResolver(
        const ModelGeneratorContext& ctx) :
                builder(ctx.getBuilder()),
                random(ctx.getRandom())
{
}


llvm::Value* DistribFunctionResolver::loadSymbolValue(
        const libsbml::FunctionDefinition* funcDef,
        const libsbml::DistribFunctionDefinitionPlugin* distribFunc,
        const llvm::ArrayRef<llvm::Value*>& args)
{
    Log(Logger::LOG_NOTICE) << "distrib function: " << funcDef->getId();

    return ConstantFP::get(builder.getContext(), APFloat(0.0));
}

} /* namespace rrllvm */

#endif /* LIBSBML_HAS_PACKAGE_DISTRIB */

