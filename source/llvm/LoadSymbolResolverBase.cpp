/*
 * LoadSymbolResolverBase.cpp
 *
 *  Created on: May 15, 2014
 *      Author: andy
 */
#include "LoadSymbolResolverBase.h"

#include "KineticLawParameterResolver.h"
#include "ASTNodeCodeGen.h"
#include "LLVMException.h"
#include "rrLogger.h"
#include <sbml/Model.h>

using namespace std;
using namespace libsbml;
using namespace llvm;

using rr::Logger;


namespace rrllvm
{

LoadSymbolResolverBase::LoadSymbolResolverBase(
        const ModelGeneratorContext& ctx) :
        modelGenContext(ctx),
        model(ctx.getModel()),
        modelSymbols(ctx.getModelSymbols()),
        modelDataSymbols(ctx.getModelDataSymbols()),
        builder(ctx.getBuilder())
{
}


llvm::Value* LoadSymbolResolverBase::loadReactionRate(
        const libsbml::Reaction* reaction)
{
    const KineticLaw *kinetic = reaction->getKineticLaw();
    const ASTNode *math = 0;
    Value *value = 0;
    ASTNode m(AST_REAL);
    if (kinetic)
    {
        math = kinetic->getMath();
    }
    else
    {
        Log(Logger::LOG_WARNING) << "Reaction " + reaction->getId() + " has no KineticLaw, it will be set to zero";
        m.setValue(0);
        math = &m;
    }

    KineticLawParameterResolver lpResolver(*this, *kinetic, builder);
    ASTNodeCodeGen astCodeGen(builder, lpResolver);

    return astCodeGen.codeGen(math);

}

void LoadSymbolResolverBase::recursiveSymbolPush(const std::string& symbol)
{
    StringStack::const_iterator prevSymbol =
                std::find(symbolStack.begin(), symbolStack.end(), symbol);
        if (prevSymbol != symbolStack.end())
        {
            std::string msg =
                "recursive assignment rule or function detected, the symbol \'";
            msg += symbol;
            msg += "\' is a parent of itself";
            Log(rr::Logger::LOG_ERROR) << msg;
            throw LLVMException(msg, __FUNC__);
        }
        symbolStack.push_back(symbol);
}

void LoadSymbolResolverBase::recursiveSymbolPop()
{
    assert(symbolStack.size() > 0 && "attempt to pop emtpy symbol stack");
    symbolStack.pop_back();
}

} /* namespace rrllvm */


