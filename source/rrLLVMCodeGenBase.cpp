/*
 * rrLLVMCodeGenBase.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: andy
 */
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMASTNodeCodeGen.h"

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
            if (species->getHasOnlySubstanceUnits())
            {
                //..return mdbuilder.create
            }
        }
        else
        {
            // floating species
            if (species->getHasOnlySubstanceUnits())
            {
                return mdbuilder.createFloatSpeciesAmtLoad(modelData, species->getId(), species->getId());
            }

        }
    }

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







    else
    {
        string msg = "Could not find requested symbol \'";
        msg += symbol;
        msg += "\' in symbol forest";
        throw LLVMException(msg, __FUNC__);
    }
    */
}
}

