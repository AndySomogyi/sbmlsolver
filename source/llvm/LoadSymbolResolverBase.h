/*
 * LoadSymbolResolverBase.h
 *
 *  Created on: May 15, 2014
 *      Author: andy
 */

#ifndef LOADSYMBOLRESOLVERBASE_H_
#define LOADSYMBOLRESOLVERBASE_H_

#include "CodeGen.h"
#include "LLVMIncludes.h"
#include "LLVMModelDataSymbols.h"
#include "LLVMModelSymbols.h"
#include "ModelGeneratorContext.h"

namespace libsbml
{
class Model;
}

namespace rrllvm
{

/**
 * provide common sbml model functionality to three kinds of resolvers.
 *
 * This has common functionality such as dealing with sbml elements
 * such as reactions, species and so forth.
 */
class LoadSymbolResolverBase: public LoadSymbolResolver
{
public:
    /**
     * shortcut to get a reaction rate, performance wise, quicker than
     * going through all the loadSymbolValue types.
     */
    llvm::Value* loadReactionRate(const libsbml::Reaction*);

    virtual void recursiveSymbolPush(const std::string& symbol);

    virtual void recursiveSymbolPop();

protected:
    LoadSymbolResolverBase(const ModelGeneratorContext &ctx);

    typedef std::list<std::string> StringStack;

    const ModelGeneratorContext &modelGenContext;
    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    const LLVMModelSymbols &modelSymbols;
    llvm::IRBuilder<> &builder;

    StringStack symbolStack;
};


} /* namespace rrllvm */

#endif /* LOADSYMBOLRESOLVERBASE_H_ */
