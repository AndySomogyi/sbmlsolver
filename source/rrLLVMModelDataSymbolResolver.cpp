/*
 * rrLLVMModelDataTermSymbolResolver.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#include "rrLLVMModelDataSymbolResolver.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMException.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

namespace rr
{

LLVMModelDataLoadSymbolResolver::LLVMModelDataLoadSymbolResolver(llvm::Value *modelData,
        const libsbml::Model *model,
        const LLVMModelSymbols &modelSymbols,
        const LLVMModelDataSymbols &modelDataSymbols,
        llvm::IRBuilder<> &builder) :
            modelData(modelData),
            model(model),
            modelSymbols(modelSymbols),
            modelDataSymbols(modelDataSymbols),
            builder(builder)
{
}

LLVMModelDataStoreSymbolResolver::LLVMModelDataStoreSymbolResolver(llvm::Value *modelData,
        const libsbml::Model *model,
        const LLVMModelSymbols &modelSymbols,
        const LLVMModelDataSymbols &modelDataSymbols,
        llvm::IRBuilder<> &builder,
        LLVMLoadSymbolResolver &resolver) :
            modelData(modelData),
            model(model),
            modelSymbols(modelSymbols),
            modelDataSymbols(modelDataSymbols),
            builder(builder),
            resolver(resolver)
{
}


llvm::Value* LLVMModelDataLoadSymbolResolver::loadSymbolValue(
        const std::string& symbol)
{
    LLVMModelDataIRBuilder mdbuilder(modelData, modelDataSymbols,
            builder);

    /*************************************************************************/
    /* AssignmentRule */
    /*************************************************************************/
    {
        LLVMSymbolForest::ConstIterator i = modelSymbols.getAssigmentRules().find(
                symbol);
        if (i != modelSymbols.getAssigmentRules().end())
        {
            return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
        }
    }

    /*************************************************************************/
    /* Species */
    /*************************************************************************/
    const Species *species = model->getSpecies(symbol);
    if (species)
    {
        Value *amt = 0;
        if (modelDataSymbols.isIndependentFloatingSpecies(symbol))
        {
            amt = mdbuilder.createFloatSpeciesAmtLoad(symbol, symbol + "_amt");
        }
        else if (modelDataSymbols.isIndependentBoundarySpecies(symbol))
        {
            amt = mdbuilder.createBoundSpeciesAmtLoad(symbol, symbol + "_amt");
        }
        else if(modelDataSymbols.hasRateRule(symbol))
        {
            amt = mdbuilder.createRateRuleLoad(symbol, symbol + "_amt");
        }
        else
        {
            string msg = string("the symbol ") + symbol + string(" appeared to "
                    "be a species, but it could not be found as an independent "
                    "species or rate rule");
            throw_llvm_exception(msg);
        }
        assert(amt);

        // now we have an amount, check to see if we need to convert to conc
        if (species->getHasOnlySubstanceUnits())
        {
            return amt;
        }
        else
        {
            // expect a concentration, need to convert amt to conc,
            // so we need to get the compartment its in, but these
            // can vary also...
            Value *comp = loadSymbolValue(species->getCompartment());
            return builder.CreateFDiv(amt, comp, symbol + "_conc");
        }
    }

    else if (modelDataSymbols.isIndependentCompartment(symbol))
    {
        return mdbuilder.createCompLoad(symbol);
    }

    else if (modelDataSymbols.isIndependentGlobalParameter(symbol))
    {
        return mdbuilder.createGlobalParamLoad(symbol);
    }

    else if (modelDataSymbols.hasRateRule(symbol))
    {
        // species conc / amt has already been taken care of at this point
        return mdbuilder.createRateRuleLoad(symbol);
    }

    string msg = "the symbol \'";
    msg += symbol;
    msg += "\' is not physically stored in the ModelData structure, "
            "it either does not exists or is defined by an assigment rule (hence it "
            "is not a terminal symbol)";

    throw_llvm_exception(msg);
    return 0;
}

llvm::Value* LLVMModelDataStoreSymbolResolver::storeSymbolValue(
        const std::string& symbol, llvm::Value *value)
{
    assert(value);

    LLVMModelDataIRBuilder mdbuilder(modelData, modelDataSymbols,
            builder);

    /*************************************************************************/
    /* AssignmentRule */
    /*************************************************************************/
    // can not store anything with an assigment rule, these are determined
    // by other independent elements.
    assert(!modelDataSymbols.hasAssignmentRule(symbol));

    /*************************************************************************/
    /* Species */
    /*************************************************************************/
    const Species *species = model->getSpecies(symbol);
    if (species)
    {
        Value *amt = 0;
        // only amounts are stored, convert to conc if required
        if (species->getHasOnlySubstanceUnits())
        {
            return amt = value;
        }
        else
        {
            // have a conc, need to convert to amt
            Value *comp = resolver.loadSymbolValue(species->getCompartment());
            amt =  builder.CreateFMul(value, comp, symbol + "_amt");
        }

        assert(amt);

        // now look where we need to store it
        if (modelDataSymbols.isIndependentFloatingSpecies(symbol))
        {
            return mdbuilder.createFloatSpeciesAmtStore(symbol, amt);
        }
        else if (modelDataSymbols.isIndependentBoundarySpecies(symbol))
        {
            return mdbuilder.createBoundSpeciesAmtStore(symbol, amt);
        }
        else if(modelDataSymbols.hasRateRule(symbol))
        {
            return mdbuilder.createRateRuleStore(symbol, amt);
        }
        else
        {
            string msg = string("the symbol ") + symbol + string(" appeared to "
                    "be a species, but it could not be found as an independent "
                    "species or rate rule");
            throw_llvm_exception(msg);
            return 0;
        }
    }

    // at this point, we have already taken care of the species amount /
    // conc conversion, rest are just plain stores.

    if (modelDataSymbols.hasRateRule(symbol))
    {
        return mdbuilder.createRateRuleStore(symbol, value);
    }

    else if (modelDataSymbols.isIndependentCompartment(symbol))
    {
        return mdbuilder.createCompStore(symbol, value);
    }

    else if (modelDataSymbols.isIndependentGlobalParameter(symbol))
    {
        return mdbuilder.createGlobalParamStore(symbol, value);
    }

    string msg = "The symbol \'";
    msg += symbol;
    msg += "\' is not physically stored in the ModelData structure, "
            "it either does not exists or is defined by an assigment rule (hence it "
            "is not a terminal symbol)";

    throw_llvm_exception(msg);
    return 0;
}

} /* namespace rr */


