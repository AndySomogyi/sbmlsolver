/*
 * EventSymbolResolver.h
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#ifndef EVENTSYMBOLRESOLVER_H_
#define EVENTSYMBOLRESOLVER_H_

#include "CodeGen.h"
#include "ModelGeneratorContext.h"

namespace rr
{

class EventSymbolResolver: public LoadSymbolResolver, public StoreSymbolResolver
{
public:
    EventSymbolResolver(llvm::Value *modelData, const libsbml::Model *model,
            const LLVMModelSymbols &modelSymbols,
            const LLVMModelDataSymbols &modelDataSymbols,
            llvm::IRBuilder<> &builder, LoadSymbolResolver &resolver);

    ~EventSymbolResolver();

    virtual llvm::Value *loadSymbolValue(const std::string& symbol);
    virtual llvm::Value *storeSymbolValue(const std::string& symbol,
            llvm::Value *value);
};

} /* namespace rr */
#endif /* EVENTSYMBOLRESOLVER_H_ */
