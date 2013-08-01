/*
 * rrLLVMModelDataSymbolResolver.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#include "rrLLVMInitialValueSymbolResolver.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMException.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

namespace rr
{

LLVMInitialValueSymbolResolver::LLVMInitialValueSymbolResolver(
        const libsbml::Model* model,
        const LLVMModelDataSymbols& modelDataSymbols,
        const LLVMModelSymbols& modelSymbols,
        llvm::IRBuilder<>& builder) :
        model(model),
        modelDataSymbols(modelDataSymbols),
        modelSymbols(modelSymbols), builder(builder)
{
}

LLVMInitialValueSymbolResolver::~LLVMInitialValueSymbolResolver()
{
}

llvm::Value* LLVMInitialValueSymbolResolver::loadSymbolValue(
        const std::string& symbol)
{
    /*************************************************************************/
    /* AssignmentRule */
    /*************************************************************************/
    {
        LLVMSymbolForest::ConstIterator i =
                modelSymbols.getAssigmentRules().find(symbol);
        if (i != modelSymbols.getAssigmentRules().end())
        {
            return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
        }
    }

    /*************************************************************************/
    /* Initial Value */
    /*************************************************************************/
    {
        LLVMSymbolForest::ConstIterator i =
                modelSymbols.getInitialValues().find(symbol);

        if (i != modelSymbols.getInitialValues().end())
        {
            return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
        }
    }

    string msg = "Could not find requested symbol \'";
    msg += symbol;
    msg += "\' in the model";
    throw_llvm_exception(msg);

    return 0;
}

} /* namespace rr */


