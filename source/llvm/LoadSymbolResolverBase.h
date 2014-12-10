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

#include <rr_unordered_map>

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

    /**
     * Flush the symbol cache. This is required in branches and switch blocks as
     * a symbol used in a previous block can not be re-used in the current block.
     */
    void flushCache();

protected:
    LoadSymbolResolverBase(const ModelGeneratorContext &ctx);

    typedef std::list<std::string> StringStack;
    typedef cxx11_ns::unordered_map<std::string, llvm::Value*> ValueMap;

    const ModelGeneratorContext &modelGenContext;
    const libsbml::Model *model;
    const LLVMModelDataSymbols &modelDataSymbols;
    const LLVMModelSymbols &modelSymbols;
    llvm::IRBuilder<> &builder;

    // check for recursive symbol definitions in rule evaluations
    StringStack symbolStack;

    // cache symbols
    ValueMap symbolCache;

    /**
     * check in the symbol cache if the symbol exists, if so return it.
     *
     * If the value is not NULL, it is cached and returned.
     *
     * Cached function args not currently supported (just return NULL).
     */
    llvm::Value* cacheValue(const std::string& symbol,
            const llvm::ArrayRef<llvm::Value*>& args,
            llvm::Value* value = NULL);
};


} /* namespace rrllvm */

#endif /* LOADSYMBOLRESOLVERBASE_H_ */
