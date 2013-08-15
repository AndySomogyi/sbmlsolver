/*
 * LocalParameterSymbolResolver.cpp
 *
 *  Created on: Aug 14, 2013
 *      Author: andy
 */

#include "KineticLawParameterResolver.h"
#include "LLVMIncludes.h"
#include "rrLogger.h"

using namespace llvm;

namespace rr
{

KineticLawParameterResolver::KineticLawParameterResolver(
        LoadSymbolResolver& parentResolver,
        const libsbml::KineticLaw& kineticLaw, llvm::IRBuilder<>& builder) :
                parentResolver(parentResolver),
                kineticLaw(kineticLaw),
                builder(builder)
{
}


llvm::Value* KineticLawParameterResolver::loadSymbolValue(
        const std::string& symbol, const llvm::ArrayRef<llvm::Value*>& args)
{
    const libsbml::Parameter *parameter =
            kineticLaw.getLocalParameter(symbol);

    if (!parameter)
    {
        parameter = kineticLaw.getParameter(symbol);
    }

    if (parameter)
    {
        Log(Logger::PRIO_DEBUG) << "found local parameter for symbol " << symbol <<
                ", value: " << parameter->getValue();

        Value *value =  ConstantFP::get(builder.getContext(),
                APFloat(parameter->getValue()));
        value->setName(symbol + "_local");
        return value;
    }

    return parentResolver.loadSymbolValue(symbol, args);
}

} /* namespace rr */
