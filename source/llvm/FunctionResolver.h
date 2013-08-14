/*
 * FunctionResolver.h
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#ifndef EVENTSYMBOLRESOLVER_H_
#define EVENTSYMBOLRESOLVER_H_

#include "CodeGen.h"
#include "ModelGeneratorContext.h"
#include <map>

namespace rr
{

/**
 * Not thread safe -- but perfectly fine if stack allocated.
 */
class FunctionResolver: public LoadSymbolResolver
{
public:
    FunctionResolver(LoadSymbolResolver& parentResolver,
            const libsbml::Model *model, llvm::IRBuilder<> &builder);

    ~FunctionResolver();

    virtual llvm::Value *loadSymbolValue(const std::string& symbol,
                const llvm::ArrayRef<llvm::Value*>& args =
                        llvm::ArrayRef<llvm::Value*>());

private:
    LoadSymbolResolver& parentResolver;
    const libsbml::Model *model;
    llvm::IRBuilder<> &builder;

    /**
     * only valid during an outer call to loadSymbolValue, otherwise
     * is null.
     */
    std::map<std::string, llvm::Value*> *symbols;
};

} /* namespace rr */
#endif /* EVENTSYMBOLRESOLVER_H_ */
