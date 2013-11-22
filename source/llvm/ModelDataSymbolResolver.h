/*
 * ModelDataSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef ModelDataSymbolResolver_H_
#define ModelDataSymbolResolver_H_

#include "CodeGen.h"
#include "LLVMIncludes.h"
#include "LLVMModelDataSymbols.h"
#include "LLVMModelSymbols.h"

namespace libsbml
{
class Model;
}

namespace rrllvm
{

class ModelDataLoadSymbolResolver: public LoadSymbolResolver
{
public:
    ModelDataLoadSymbolResolver(llvm::Value *modelData,
            const libsbml::Model *model, const LLVMModelSymbols &modelSymbols,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder);

    virtual ~ModelDataLoadSymbolResolver() {};

    virtual llvm::Value *loadSymbolValue(const std::string& symbol,
            const llvm::ArrayRef<llvm::Value*>& args =
                    llvm::ArrayRef<llvm::Value*>());

private:
    llvm::Value *modelData;
    const libsbml::Model *model;
    const LLVMModelSymbols &modelSymbols;
    const LLVMModelDataSymbols &modelDataSymbols;
    llvm::IRBuilder<> &builder;
};

class ModelDataStoreSymbolResolver: public StoreSymbolResolver
{
public:
    ModelDataStoreSymbolResolver(llvm::Value *modelData,
            const libsbml::Model *model, const LLVMModelSymbols &modelSymbols,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder,
            LoadSymbolResolver &resolver);

    virtual ~ModelDataStoreSymbolResolver() {};

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
#endif /* rrLLVMModelDataSymbolResolver_H_ */
