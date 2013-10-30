/*
 * ModelInitialValueSymbolResolver.cpp
 *
 *  Created on: Oct 29, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "ModelInitialValueSymbolResolver.h"
#include "ASTNodeCodeGen.h"
#include "LLVMException.h"
#include "FunctionResolver.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

namespace rrllvm
{

ModelInitialValueSymbolResolver::ModelInitialValueSymbolResolver(
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

ModelInitialValueSymbolResolver::~ModelInitialValueSymbolResolver()
{
}

llvm::Value* ModelInitialValueSymbolResolver::loadSymbolValue(const std::string& symbol,
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
    /* Initial AssignmentRule */
    /*************************************************************************/
    {
        SymbolForest::ConstIterator i =
                modelSymbols.getInitialAssignmentRules().find(symbol);
        if (i != modelSymbols.getInitialAssignmentRules().end())
        {
            return ASTNodeCodeGen(builder, *this).codeGen(i->second);
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


