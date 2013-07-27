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

LLVMModelDataSymbolResolver::LLVMModelDataSymbolResolver(llvm::Value *modelData,
        const libsbml::Model *model,
        const LLVMModelSymbols &modelSymbols,
        const LLVMModelDataSymbols &modelDataSymbols,
        llvm::IRBuilder<> &builder) :
                LLVMBasicSymbolResolver(model,
                        modelSymbols,
                        modelDataSymbols,
                        builder,
                        terminal),
                terminal(modelData,model,modelDataSymbols,builder)
{
}

LLVMModelDataTermSymbolResolver::LLVMModelDataTermSymbolResolver(llvm::Value *modelData,
        const libsbml::Model *model,
        const LLVMModelDataSymbols &modelDataSymbols,
        llvm::IRBuilder<> &builder) :
            modelData(modelData),
            model(model),
            modelDataSymbols(modelDataSymbols),
            builder(builder)
{
}

LLVMModelDataTermSymbolResolver::~LLVMModelDataTermSymbolResolver()
{
}

llvm::Value* LLVMModelDataTermSymbolResolver::symbolValue(const std::string& symbol)
{
    LLVMModelDataIRBuilder mdbuilder(modelData, modelDataSymbols, builder);

    const SBase *element = const_cast<Model*>(model)->getElementBySId(symbol);

    /*************************************************************************/
    /* Species */
    /*************************************************************************/
    const Species *species = dynamic_cast<const Species*>(element);
    if (species)
    {
        if (species->getBoundaryCondition())
        {
            // boundary species
            return mdbuilder.createBoundSpeciesAmtLoad(species->getId(),
                    species->getId() + "_amt");
        }
        else
        {
            // floating species
            return mdbuilder.createFloatSpeciesAmtLoad(species->getId(),
                    species->getId() + "_amt");
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

    string msg = "Could not find requested symbol \'";
    msg += symbol;
    msg += "\' in the model";
    throw_llvm_exception(msg);

    return 0;
}

} /* namespace rr */


