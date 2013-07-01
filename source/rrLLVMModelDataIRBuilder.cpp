/*
 * rrLLVMModelDataIRBuilder.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */
#include "rrLLVMModelDataIRBuilder.h"
#include "lsLibStructural.h"
#include "rrLLVMException.h"
#include "rrOSSpecifics.h"

#include <sbml/Model.h>
#include <sbml/SBMLDocument.h>

#include <llvm/Support/raw_ostream.h>

#include <string>
#include <vector>
#include <sstream>

using namespace libsbml;
using namespace llvm;
using namespace std;

namespace rr
{


LLVMModelDataIRBuilder::LLVMModelDataIRBuilder(const LLVMModelDataSymbols &symbols,
        IRBuilder<> *b) :
        symbols(symbols),
        builder(b),
        cachedStructValue(0)
{

}

LLVMModelDataIRBuilder::~LLVMModelDataIRBuilder()
{
    // TODO Auto-generated destructor stub
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesConcEP(llvm::Value* s,
        const std::string& id)
{
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciedAmtEP(llvm::Value* s,
        const std::string& id)
{
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesConcFromAmtLoad(
        llvm::Value* s, const std::string& id)
{
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesAmtFromConcLoad(
        llvm::Value* s, const std::string& id)
{
}

llvm::Value* LLVMModelDataIRBuilder::createGlobalParamEP(llvm::Value* s,
        const std::string& id)
{
    validateStruct(s, __FUNC__);
    int index = symbols.getGlobalParameterIndex(id);
    Value *gep = createGEP(s, GlobalParameters);
    Value *load = builder->CreateLoad(gep);
    return builder->CreateConstGEP1_32(load, index);
}


llvm::Value* LLVMModelDataIRBuilder::createGEP(llvm::Value* s,
        ModelDataFields field)
{
    Value *result = 0;
    validateStruct(s, __FUNC__);
    return builder->CreateStructGEP(s, field, "");
}


void LLVMModelDataIRBuilder::validateStruct(llvm::Value* s, const char* funcName)
{
    if (!builder)
    {
        string err = "error in ";
        err += funcName;
        err += ", builder is null";
        throw LLVMException(err);
    }

    if (s != cachedStructValue)
    {
        StructType *structType = dyn_cast<StructType>(s->getType());
        if (structType)
        {
            if (structType->getName().compare(LLVMModelDataSymbols::ModelDataName) == 0)
            {
                cachedStructValue = s;
                return;
            }
        }
        std::string str;
        raw_string_ostream err(str);
        err << "error in " << funcName << ", "
                << "Invalid argument type, expected " << LLVMModelDataSymbols::ModelDataName
                << ", but received ";
        s->getType()->print(err);
        throw LLVMException(err.str());
    }
}

} /* namespace rr */
