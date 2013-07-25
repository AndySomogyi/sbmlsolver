/*
 * rrLLVMModelDataSymbolResolver.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#include "rrLLVMModelDataSymbolResolver.h"

namespace rr
{

LLVMModelDataSymbolResolver::LLVMModelDataSymbolResolver()
{
    // TODO Auto-generated constructor stub

}

LLVMModelDataSymbolResolver::~LLVMModelDataSymbolResolver()
{
    // TODO Auto-generated destructor stub
}

llvm::Value* LLVMCodeGenBase::symbolValue(const std::string& symbol)
{
    LLVMModelDataIRBuilder mdbuilder(modelData, dataSymbols, builder);

    LLVMSymbolForest::ConstIterator i = modelSymbols.getAssigmentRules().find(
            symbol);
    if (i != modelSymbols.getAssigmentRules().end())
    {
        return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
    }

    const SBase *element = const_cast<Model*>(model)->getElementBySId(symbol);

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
                Value *comp = species->getCompartment();
                return mdbuilder.createBoundSpeciesConcFromAmtLoad(
                        species->getId(), species->getId() + "_conc");
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
                // expect a concentration, need to convert amt to conc
                return mdbuilder.createFloatSpeciesConcFromAmtLoad(
                        species->getId(), species->getId() + "_conc");
            }
        }
    }

    const Parameter* param = dynamic_cast<const Parameter*>(element);
    if (param)
    {
        return mdbuilder.createGlobalParamLoad(param->getId(), param->getId());
    }

    const Compartment* comp = dynamic_cast<const Compartment*>(element);
    if (comp)
    {
        return mdbuilder.createCompLoad(comp->getId(), comp->getId());
    }

    string msg = "Could not find requested symbol \'";
    msg += symbol;
    msg += "\' in the model";
    throw LLVMException(msg, __FUNC__);


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
