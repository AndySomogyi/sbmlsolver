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
        llvm::Value *modelData,
        const libsbml::Model *model,
        const LLVMModelSymbols &modelSymbols,
        const LLVMModelDataSymbols &modelDataSymbols,
        llvm::IRBuilder<> &builder) :
            LoadSymbolResolverBase(model, modelSymbols, modelDataSymbols, builder),
            modelData(modelData)
{
}


ModelInitialValueStoreSymbolResolver::ModelInitialValueStoreSymbolResolver(llvm::Value *modelData,
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


llvm::Value* ModelInitialValueSymbolResolver::loadSymbolValue(
        const std::string& symbol,
        const llvm::ArrayRef<llvm::Value*>& args)
{
    ModelDataIRBuilder mdbuilder(modelData, modelDataSymbols, builder);

    /*************************************************************************/
    /* time */
    /*************************************************************************/
    if (symbol.compare(SBML_TIME_SYMBOL) == 0)
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
            recursiveSymbolPush(symbol);
            Value* result =  ASTNodeCodeGen(builder, *this).codeGen(i->second);
            recursiveSymbolPop();
            return result;
        }
    }

    /*************************************************************************/
    /* AssignmentRule */
    /*************************************************************************/
    if (!modelDataSymbols.isConservedMoietySpecies(symbol))
    {
        SymbolForest::ConstIterator i =
                modelSymbols.getAssigmentRules().find(symbol);
        if (i != modelSymbols.getAssigmentRules().end())
        {
            return ASTNodeCodeGen(builder, *this).codeGen(i->second);
        }
    }

    if (modelDataSymbols.isIndependentInitFloatingSpecies(symbol))
    {
        const Species *species = model->getSpecies(symbol);

        assert(species);

        Value *amt = mdbuilder.createInitFloatSpeciesAmtLoad(symbol, symbol + "_amt");

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

    else if (modelDataSymbols.isIndependentInitCompartment(symbol))
    {
        return mdbuilder.createInitCompLoad(symbol);
    }


    else if (modelDataSymbols.isIndependentInitGlobalParameter(symbol))
    {
        return mdbuilder.createInitGlobalParamLoad(symbol);
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

    /*************************************************************************/
    /* Reaction Rate */
    /*************************************************************************/
    const Reaction* reaction = model->getReaction(symbol);
    if (reaction)
    {
        return loadReactionRate(reaction);
    }

    /*************************************************************************/
    /* rate of change */
    /*************************************************************************/
    {
        std::string::size_type tick = symbol.find("'");
        if (tick != symbol.npos)
        {
            std::string rateSymbol = symbol.substr(0, tick);

            const Species *species = model->getSpecies(rateSymbol);
            if (species)
            {
                Value *amtRate = 0;
                if (modelDataSymbols.isIndependentFloatingSpecies(rateSymbol))
                {
                    amtRate = mdbuilder.createFloatSpeciesAmtRateComputeLoad(
                            rateSymbol, rateSymbol + "_amt");
                }
                else
                {
                    string msg = string("the rate symbol ") + rateSymbol + string(" appeared to "
                            "be a species, but it could not be found as an independent "
                            "species or rate rule");
                    throw_llvm_exception(msg);
                }
                assert(amtRate);

                // now we have an amount, check to see if we need to convert to conc
                if (species->getHasOnlySubstanceUnits())
                {
                    return amtRate;
                }
                else
                {
                    // expect a concentration, need to convert amt to conc,
                    // so we need to get the compartment its in, but these
                    // can vary also...
                    Value *comp = loadSymbolValue(species->getCompartment());
                    return builder.CreateFDiv(amtRate, comp, rateSymbol + "_conc");
                }
            }
            else
            {
                std::string msg = "rate of change function currently only implemented for floating species, ";
                msg += "the symbol " + rateSymbol + " is not a floating species";
                throw_llvm_exception(msg);
            }
        }
    }



    string msg = "Could not find requested symbol \'";
    msg += symbol;
    msg += "\' in the model";
    throw_llvm_exception(msg);

    return 0;
}


llvm::Value* ModelInitialValueStoreSymbolResolver::storeSymbolValue(
        const std::string& symbol, llvm::Value *value)
{
    assert(value);

    ModelDataIRBuilder mdbuilder(modelData, modelDataSymbols,
            builder);


    if (modelDataSymbols.isIndependentInitFloatingSpecies(symbol))
    {
        const Species *species = model->getSpecies(symbol);
        assert(species);

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

        return mdbuilder.createInitFloatSpeciesAmtStore(symbol, amt);
    }

    else if (modelDataSymbols.isIndependentInitCompartment(symbol))
    {
        return mdbuilder.createInitCompStore(symbol, value);
    }

    else if (modelDataSymbols.isIndependentInitGlobalParameter(symbol))
    {
        return mdbuilder.createInitGlobalParamStore(symbol, value);
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


