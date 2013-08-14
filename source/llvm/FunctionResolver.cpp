/*
 * FunctionResolver.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#include "llvm/FunctionResolver.h"

namespace rr
{

using namespace libsbml;

rr::FunctionResolver::FunctionResolver(const LoadSymbolResolver& parentResolver,
        const libsbml::Model* model, llvm::IRBuilder<>& builder) :
                parentResolver(parentResolver),
                model(model),
                builder(builder),
                symbols(0)
{
}

rr::FunctionResolver::~FunctionResolver()
{
}

llvm::Value* rr::FunctionResolver::loadSymbolValue(const std::string& symbol,
        const llvm::ArrayRef<llvm::Value*>& args)
{
    const FunctionDefinition *funcDef = model->getListOfFunctionDefinitions()->get(symbol);

    if (funcDef)
    {
        const ASTNode *math = funcDef->getMath();


    }

    return 0;
}

} /* namespace rr */
