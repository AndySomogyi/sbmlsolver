/*
 * rrLLVMCodeGenBase.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: andy
 */
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMException.h"

namespace rr
{
using namespace libsbml;
using namespace llvm;
using namespace std;

llvm::Value* LLVMCodeGenBase::symbolValue(const std::string& symbol, Value *modelData)
{
    LLVMModelDataIRBuilder mdbuilder(dataSymbols, builder);

    LLVMSymbolForest::ConstIterator i = modelSymbols.getAssigmentRules().find(symbol);
    if (i != modelSymbols.getAssigmentRules().end())
    {
        return LLVMASTNodeCodeGen(*builder, *this).codeGen(i->second);
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
                return mdbuilder.createBoundSpeciesAmtLoad(modelData,
                        species->getId(), species->getId() + "_amt");
            }
            else
            {
                // expect a concentration, need to convert amt to conc
                return mdbuilder.createBoundSpeciesConcFromAmtLoad(modelData,
                        species->getId(), species->getId() + "_conc");
            }
        }
        else
        {
            // floating species
            if (species->getHasOnlySubstanceUnits())
            {
                // expect an amount, we're good to go
                return mdbuilder.createFloatSpeciesAmtLoad(modelData,
                        species->getId(), species->getId() + "_amt");
            }
            else
            {
                // expect a concentration, need to convert amt to conc
                return mdbuilder.createFloatSpeciesConcFromAmtLoad(modelData,
                        species->getId(), species->getId() + "_conc");
            }
        }
    }

    const Parameter* param = dynamic_cast<const Parameter*>(element);
    if (param)
    {
        return mdbuilder.createGlobalParamLoad(modelData, param->getId(), param->getId());
    }

    const Compartment* comp = dynamic_cast<const Compartment*>(element);
    if (comp)
    {
        return mdbuilder.createCompLoad(modelData, comp->getId(), comp->getId());
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
}

