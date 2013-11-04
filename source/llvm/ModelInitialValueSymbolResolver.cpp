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
            modelData(modelData),
            model(model),
            modelSymbols(modelSymbols),
            modelDataSymbols(modelDataSymbols),
            builder(builder)
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


llvm::Value* ModelInitialValueStoreSymbolResolver::storeSymbolValue(
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


    if (modelDataSymbols.isIndependentInitFloatingSpecies(symbol))
    {
        string id = modelDataSymbols.getInitSymbolId(symbol);
        const Species *species = model->getSpecies(id);
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
            Value *comp = resolver.loadSymbolValue(
                    modelDataSymbols.getInitSymbol(species->getCompartment()));
            amt =  builder.CreateFMul(value, comp, symbol + "_amt");
        }

        assert(amt);

        return mdbuilder.createInitFloatSpeciesAmtStore(symbol, amt);
    }

    else if (modelDataSymbols.isIndependentInitCompartment(symbol))
    {
        return mdbuilder.createInitCompStore(symbol, value);
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


