/*
 * InitialValueSymbolResolver.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "InitialValueSymbolResolver.h"
#include "ASTNodeCodeGen.h"
#include "LLVMException.h"
#include "FunctionResolver.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

namespace rrllvm
{

InitialValueSymbolResolver::InitialValueSymbolResolver(
        const libsbml::Model* model,
        const LLVMModelDataSymbols& modelDataSymbols,
        const LLVMModelSymbols& modelSymbols,
        llvm::IRBuilder<>& builder) :
        model(model),
        modelDataSymbols(modelDataSymbols),
        modelSymbols(modelSymbols),
        builder(builder)
{
}

InitialValueSymbolResolver::~InitialValueSymbolResolver()
{
}

llvm::Value* InitialValueSymbolResolver::loadSymbolValue(const std::string& symbol,
        const llvm::ArrayRef<llvm::Value*>& args)
{
    /*************************************************************************/
    /* time */
    /*************************************************************************/
    if (symbol.compare("\time") == 0)
    {
        return ConstantFP::get(builder.getContext(), APFloat(0.0));
    }

    /*************************************************************************/
    /* Function */
    /*************************************************************************/
    {
        Value *funcVal =
            FunctionResolver(*this, model, builder).loadSymbolValue(symbol, args);
        if (funcVal)
        {
            return funcVal;
        }
    }

    /*************************************************************************/
    /* AssignmentRule */
    /*************************************************************************/
    {
        SymbolForest::ConstIterator i =
                modelSymbols.getAssigmentRules().find(symbol);
        if (i != modelSymbols.getAssigmentRules().end())
        {
            return ASTNodeCodeGen(builder, *this).codeGen(i->second);
        }
    }

    /*************************************************************************/
    /* Initial Value */
    /*************************************************************************/
    {
        SymbolForest::ConstIterator i =
                modelSymbols.getInitialValues().find(symbol);

        if (i != modelSymbols.getInitialValues().end())
        {
            return ASTNodeCodeGen(builder, *this).codeGen(i->second);
        }
    }

    string msg = "Could not find requested symbol \'";
    msg += symbol;
    msg += "\' in the model";
    throw_llvm_exception(msg);

    return 0;
}

} /* namespace rr */


