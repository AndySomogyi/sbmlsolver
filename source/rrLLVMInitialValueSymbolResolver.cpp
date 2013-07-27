/*
 * rrLLVMModelDataSymbolResolver.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#include "rrLLVMInitialValueSymbolResolver.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMException.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

namespace rr
{

LLVMInitialValueSymbolResolver::LLVMInitialValueSymbolResolver(
        const libsbml::Model* model,
        const LLVMModelDataSymbols& modelDataSymbols,
        const LLVMModelSymbols& modelSymbols,
        llvm::IRBuilder<>& builder) :
                LLVMBasicSymbolResolver(model,
                        modelSymbols,
                        modelDataSymbols,
                        builder,
                        terminal),
                terminal(model,
                        modelDataSymbols,
                        modelSymbols,
                        builder,
                        *this)
{
}

LLVMInitialValueTermSymbolResolver::LLVMInitialValueTermSymbolResolver(
        const libsbml::Model* model,
        const LLVMModelDataSymbols& modelDataSymbols,
        const LLVMModelSymbols& modelSymbols, llvm::IRBuilder<>& builder,
        LLVMSymbolResolver& parent) :
                model(model),
                modelDataSymbols(modelDataSymbols),
                modelSymbols(modelSymbols),
                builder(builder),
                parent(parent)
{
}

LLVMInitialValueSymbolResolver::~LLVMInitialValueSymbolResolver()
{
}

LLVMInitialValueTermSymbolResolver::~LLVMInitialValueTermSymbolResolver()
{
}

llvm::Value* LLVMInitialValueTermSymbolResolver::symbolValue(const std::string& symbol)
{
    //LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialAssigments().find(symbol);
    //if (i != modelSymbols.getInitialAssigments().end())
    //{
    //    return LLVMASTNodeCodeGen(builder, parent).codeGen(i->second);
   // }

    const SBase *element = const_cast<Model*>(model)->getElementBySId(symbol);

    /*************************************************************************/
    /* Species */
    /*************************************************************************/
    const Species *species = dynamic_cast<const Species*>(element);
    if (species)
    {
        // treat everything as an amount, the BasicSymbolResolver takes
        // care of converting to a concentration, here we
        // just build an AST for get value as an amount
        ASTNode *amt = 0;
        if (species->isSetInitialConcentration())
        {
            ASTNode *conc = new ASTNode(AST_REAL);
            conc->setValue(species->getInitialConcentration());

            amt = new ASTNode(AST_TIMES);
            amt->addChild(conc);
            ASTNode *comp = new ASTNode(AST_NAME);
            comp->setName(species->getCompartment().c_str());
            amt->addChild(comp);
        }
        else if (species->isSetInitialAmount())
        {
            amt = new ASTNode(AST_REAL);
            amt->setValue(species->getInitialAmount());
        }
        Value *amtVal = LLVMASTNodeCodeGen(builder, parent).codeGen(amt);
        amtVal->setName(symbol + "_amt");
        delete amt;
        return amtVal;
    }

    /*************************************************************************/
    /* Parameter */
    /*************************************************************************/
    const Parameter* param = dynamic_cast<const Parameter*>(element);
    if (param)
    {
        ASTNode *paramNode = new ASTNode(AST_REAL);
        paramNode->setValue(param->getValue());
        Value *paramVal = LLVMASTNodeCodeGen(builder, parent).codeGen(paramNode);
        delete paramNode;
        return paramVal;
    }

    /*************************************************************************/
    /* Compartment */
    /*************************************************************************/
    const Compartment* comp = dynamic_cast<const Compartment*>(element);
    if (comp)
    {
        ASTNode *compNode = new ASTNode(AST_REAL);
        compNode->setValue(comp->getVolume());
        Value *paramVal = LLVMASTNodeCodeGen(builder, parent).codeGen(compNode);
        delete compNode;
        return paramVal;
    }

    string msg = "Could not find requested symbol \'";
    msg += symbol;
    msg += "\' in the model";
    throw_llvm_exception(msg);

    return 0;
}




} /* namespace rr */

