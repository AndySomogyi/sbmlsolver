/*
 * rrLLVMModelDataSymbolResolver.h
 *
 *  Created on: Jul 25, 2013
 *      Author: andy
 */

#ifndef rrLLVMModelDataSymbolResolver_H_
#define rrLLVMModelDataSymbolResolver_H_

#include "rrLLVMCodeGen.h"
#include "rrLLVMIncludes.h"
#include "rrLLVMModelDataSymbols.h"
#include "rrLLVMModelSymbols.h"

namespace libsbml
{
class Model;
}

namespace rr
{

class LLVMModelDataLoadSymbolResolver: public LLVMLoadSymbolResolver
{
public:
    LLVMModelDataLoadSymbolResolver(llvm::Value *modelData,
            const libsbml::Model *model, const LLVMModelSymbols &modelSymbols,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder);

    virtual ~LLVMModelDataLoadSymbolResolver() {};

    virtual llvm::Value *loadSymbolValue(const std::string& symbol);

private:

    llvm::Value *modelData;
    const libsbml::Model *model;
    const LLVMModelSymbols &modelSymbols;
    const LLVMModelDataSymbols &modelDataSymbols;
    llvm::IRBuilder<> &builder;

};

class LLVMModelDataStoreSymbolResolver: public LLVMStoreSymbolResolver
{
public:
    LLVMModelDataStoreSymbolResolver(llvm::Value *modelData,
            const libsbml::Model *model, const LLVMModelSymbols &modelSymbols,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder,
            LLVMLoadSymbolResolver &resolver);

    virtual ~LLVMModelDataStoreSymbolResolver() {};

    virtual llvm::Value *storeSymbolValue(const std::string& symbol,
            llvm::Value *value);
private:

    llvm::Value *modelData;
    const libsbml::Model *model;
    const LLVMModelSymbols &modelSymbols;
    const LLVMModelDataSymbols &modelDataSymbols;
    llvm::IRBuilder<> &builder;
    LLVMLoadSymbolResolver &resolver;

};

} /* namespace rr */
#endif /* rrLLVMModelDataSymbolResolver_H_ */
