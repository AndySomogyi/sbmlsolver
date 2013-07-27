/*
 * rrLLVMModelDataSymbolResolver.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#include "rrLLVMBasicSymbolResolver.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMException.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

namespace rr
{


LLVMBasicSymbolResolver::LLVMBasicSymbolResolver(const libsbml::Model *model,
        const LLVMModelSymbols &modelSymbols,
        const LLVMModelDataSymbols &modelDataSymbols,
        llvm::IRBuilder<> &builder,
        LLVMSymbolResolver &terminal) :
            model(model),
            modelSymbols(modelSymbols),
            modelDataSymbols(modelDataSymbols),
            builder(builder),
            terminal(terminal)
{
}

LLVMBasicSymbolResolver::~LLVMBasicSymbolResolver()
{
}

llvm::Value* LLVMBasicSymbolResolver::symbolValue(const std::string& symbol)
{
    /*************************************************************************/
    /* AssignmentRule */
    /*************************************************************************/
    LLVMSymbolForest::ConstIterator i = modelSymbols.getAssigmentRules().find(
            symbol);
    if (i != modelSymbols.getAssigmentRules().end())
    {
        return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
    }

    const SBase *element = const_cast<Model*>(model)->getElementBySId(symbol);

    /*************************************************************************/
    /* Species */
    /*************************************************************************/
    const Species *species = dynamic_cast<const Species*>(element);
    if (species)
    {
        Value *amt = terminal.symbolValue(symbol);
        if (species->getHasOnlySubstanceUnits())
        {
            // expect an amount, we're good to go
            return amt;
        }
        else
        {
            // expect a concentration, need to convert amt to conc,
            // so we need to get the compartment its in, but these
            // can vary also...
            Value *comp = symbolValue(species->getCompartment());
            return builder.CreateFDiv(amt, comp, symbol + "_conc");
        }
    }


    /*************************************************************************/
    /* Look in tail */
    /*************************************************************************/
    return terminal.symbolValue(symbol);
}


} /* namespace rr */


