/*
 * EventSymbolResolver.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#include "llvm/EventSymbolResolver.h"

namespace rr
{


EventSymbolResolver::~EventSymbolResolver()
{
}

rr::EventSymbolResolver::EventSymbolResolver(llvm::Value* modelData,
        const libsbml::Model* model, const LLVMModelSymbols& modelSymbols,
        const LLVMModelDataSymbols& modelDataSymbols,
        llvm::IRBuilder<>& builder, LoadSymbolResolver& resolver)
{
}

llvm::Value* rr::EventSymbolResolver::loadSymbolValue(const std::string& symbol)
{
    return 0;
}

llvm::Value* rr::EventSymbolResolver::storeSymbolValue(
        const std::string& symbol, llvm::Value* value)
{
    return 0;
}

} /* namespace rr */
