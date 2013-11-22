/*
 * rrLLVMModelDataTermSymbolResolver.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "ModelDataSymbolResolver.h"
#include "ASTNodeCodeGen.h"
#include "LLVMException.h"
#include "FunctionResolver.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

namespace rrllvm
{

ModelDataLoadSymbolResolver::ModelDataLoadSymbolResolver(llvm::Value *modelData,
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

ModelDataStoreSymbolResolver::ModelDataStoreSymbolResolver(llvm::Value *modelData,
        const libsbml::Model *model,
        const LLVMModelSymbols &modelSymbols,
        const LLVMModelDataSymbols &modelDataSymbols,
        llvm::IRBuilder<> &builder,
        LoadSymbolResolver &resolver) :
            modelData(modelData),
            model(model),
            modelSymbols(modelSymbols),
            modelDataSymbols(modelDataSymbols),
            builder(builder),
            resolver(resolver)
{
}


llvm::Value* ModelDataLoadSymbolResolver::loadSymbolValue(const std::string& symbol,
        const llvm::ArrayRef<llvm::Value*>& args)
{
    ModelDataIRBuilder mdbuilder(modelData, modelDataSymbols,
            builder);

    /*************************************************************************/
    /* time */
    /*************************************************************************/
    if (symbol.compare("\time") == 0)
    {
        Value *timeEP = mdbuilder.createGEP(Time);
        Value *time = builder.CreateLoad(timeEP, "time");
        return time;
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
        SymbolForest::ConstIterator i = modelSymbols.getAssigmentRules().find(
                symbol);
        if (i != modelSymbols.getAssigmentRules().end())
        {
            return ASTNodeCodeGen(builder, *this).codeGen(i->second);
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
            amt = mdbuilder.createRateRuleValueLoad(symbol, symbol + "_amt");
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
        return mdbuilder.createRateRuleValueLoad(symbol);
    }

    else if (modelDataSymbols.isNamedSpeciesReference(symbol))
    {
        const LLVMModelDataSymbols::SpeciesReferenceInfo &info =
                modelDataSymbols.getNamedSpeciesReferenceInfo(symbol);

        Value *value = mdbuilder.createStoichiometryLoad(info.row, info.column, symbol);

        if (info.type == LLVMModelDataSymbols::Reactant)
        {
            // its consumed in the reaction, so has a negative in the stoich
            // matrix
            Value *negOne = ConstantFP::get(builder.getContext(), APFloat(-1.0));
            negOne->setName("neg_one");
            value = builder.CreateFMul(negOne, value, "neg_" + symbol);
        }

        return value;
    }

    string msg = "the symbol \'";
    msg += symbol;
    msg += "\' is not physically stored in the ModelData structure, "
            "it either does not exists or is defined by an assigment rule (hence it "
            "is not a terminal symbol)";

    throw_llvm_exception(msg);
    return 0;
}

llvm::Value* ModelDataStoreSymbolResolver::storeSymbolValue(
        const std::string& symbol, llvm::Value *value)
{
    assert(value);

    ModelDataIRBuilder mdbuilder(modelData, modelDataSymbols,
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
            amt = value;
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
            return mdbuilder.createRateRuleValueStore(symbol, amt);
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
        return mdbuilder.createRateRuleValueStore(symbol, value);
    }

    else if (modelDataSymbols.isIndependentCompartment(symbol))
    {
        return mdbuilder.createCompStore(symbol, value);
    }

    else if (modelDataSymbols.isIndependentGlobalParameter(symbol))
    {
        return mdbuilder.createGlobalParamStore(symbol, value);
    }

    else if (modelDataSymbols.isNamedSpeciesReference(symbol))
    {
        const LLVMModelDataSymbols::SpeciesReferenceInfo &info =
                modelDataSymbols.getNamedSpeciesReferenceInfo(symbol);

        if (info.type == LLVMModelDataSymbols::Reactant)
        {
            // its consumed in the reaction, so has a negative in the stoich
            // matrix
            Value *negOne = ConstantFP::get(builder.getContext(), APFloat(-1.0));
            negOne->setName("neg_one");
            value = builder.CreateFMul(negOne, value, "neg_" + symbol);
        }

        return mdbuilder.createStoichiometryStore(info.row, info.column, value);
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


