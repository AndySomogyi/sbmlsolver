/*
 * rrLLVMModelDataSymbolResolver.cpp
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

LLVMBasicSymbolResolver::LLVMBasicSymbolResolver(const libsbml::Model *model,
        const LLVMModelDataSymbols &modelDataSymbols,
        const LLVMModelSymbols &modelSymbols, llvm::IRBuilder<> &builder,
        LLVMSymbolResolver *tail) :
            model(model),
            modelDataSymbols(modelDataSymbols),
            modelSymbols(modelSymbols), builder(builder), tail(tail)
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
        if (species->getBoundaryCondition())
        {
            // floating species
            if (species->getHasOnlySubstanceUnits())
            {
                // expect an amount, we're good to go
                return mdbuilder.createBoundSpeciesAmtLoad(species->getId(),
                        species->getId() + "_amt");
            }
            else
            {
                // expect a concentration, need to convert amt to conc,
                // so we need to get the compartment its in, but these
                // can vary also...
                Value *amt = mdbuilder.createBoundSpeciesAmtLoad(symbol);
                Value *comp = symbolValue(species->getCompartment());
                return builder.CreateFDiv(amt, comp, symbol + "_conc");
            }
        }
        else
        {
            // floating species
            if (species->getHasOnlySubstanceUnits())
            {
                // expect an amount, we're good to go
                return mdbuilder.createFloatSpeciesAmtLoad(species->getId(),
                        species->getId() + "_amt");
            }
            else
            {
                // expect a concentration, need to convert amt to conc,
                // so we need to get the compartment its in, but these
                // can vary also...
                Value *amt = mdbuilder.createFloatSpeciesAmtLoad(symbol);
                Value *comp = symbolValue(species->getCompartment());
                return builder.CreateFDiv(amt, comp, symbol + "_conc");
            }
        }
    }

    /*************************************************************************/
    /* Parameter */
    /*************************************************************************/
    const Parameter* param = dynamic_cast<const Parameter*>(element);
    if (param)
    {
        return mdbuilder.createGlobalParamLoad(param->getId(), param->getId());
    }

    /*************************************************************************/
    /* Compartment */
    /*************************************************************************/
    const Compartment* comp = dynamic_cast<const Compartment*>(element);
    if (comp)
    {
        return mdbuilder.createCompLoad(comp->getId(), comp->getId());
    }

    /*************************************************************************/
    /* Look in tail */
    /*************************************************************************/
    if (tail)
    {
        return tail->symbolValue(symbol);
    }

    string msg = "Could not find requested symbol \'";
    msg += symbol;
    msg += "\' in the model";
    throw_llvm_exception(msg);

    return 0;


    /*
     * if (id.empty()) return NULL;
  SBase* obj = mFunctionDefinitions.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mUnitDefinitions.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mCompartmentTypes.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mSpeciesTypes.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mCompartments.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mSpecies.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mParameters.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mReactions.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mInitialAssignments.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mRules.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mConstraints.getElementBySId(id);
  if (obj != NULL) return obj;
  obj = mEvents.getElementBySId(id);
  if (obj != NULL) return obj;
     */



    /*








    */
}

} /* namespace rr */


