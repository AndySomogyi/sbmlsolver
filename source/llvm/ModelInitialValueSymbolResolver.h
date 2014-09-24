/*
 * ModelInitialValueSymbolResolver.h
 *
 *  Created on: Oct 29, 2013
 *      Author: andy
 */

#ifndef ModelInitialValueSymbolResolver_H_
#define ModelInitialValueSymbolResolver_H_

#include "LoadSymbolResolverBase.h"
#include "LLVMIncludes.h"
#include "LLVMModelDataSymbols.h"
#include "LLVMModelSymbols.h"

namespace libsbml
{
class Model;
}

namespace rrllvm
{

/**
 * pulls values from the initial conditions data blocks
 */
class ModelInitialValueSymbolResolver: public LoadSymbolResolverBase
{
public:

    ModelInitialValueSymbolResolver(llvm::Value *modelData,
            const ModelGeneratorContext& ctx);

    virtual ~ModelInitialValueSymbolResolver() {};

    virtual llvm::Value *loadSymbolValue(const std::string& symbol,
            const llvm::ArrayRef<llvm::Value*>& args =
                    llvm::ArrayRef<llvm::Value*>());

private:
    llvm::Value *modelData;
};


class ModelInitialValueStoreSymbolResolver : public StoreSymbolResolver
{
public:
    ModelInitialValueStoreSymbolResolver(llvm::Value *modelData,
            const libsbml::Model *model, const LLVMModelSymbols &modelSymbols,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder,
            LoadSymbolResolver &resolver);

    virtual ~ModelInitialValueStoreSymbolResolver() {};

    virtual llvm::Value *storeSymbolValue(const std::string& symbol,
            llvm::Value *value);
private:

    llvm::Value *modelData;
    const libsbml::Model *model;
    const LLVMModelSymbols &modelSymbols;
    const LLVMModelDataSymbols &modelDataSymbols;
    llvm::IRBuilder<> &builder;
    LoadSymbolResolver &resolver;

};

} /* namespace rr */
#endif /* ModelInitialValueSymbolResolver_H_ */
