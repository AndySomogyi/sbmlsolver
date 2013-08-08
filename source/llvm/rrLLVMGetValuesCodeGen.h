/*
 * rrLLVMGetBoundarySpeciesAmountsCodeGen.h
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */

#ifndef RRLLVMGETBOUNDARYSPECIESAMOUNTSCODEGEN_H_
#define RRLLVMGETBOUNDARYSPECIESAMOUNTSCODEGEN_H_

#include "rrLLVMCodeGenBase.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include "rrLLVMGetValueCodeGenBase.h"
#include <sbml/Model.h>

namespace rr
{

class LLVMGetBoundarySpeciesAmountCodeGen: public
    LLVMGetValueCodeGenBase<LLVMGetBoundarySpeciesAmountCodeGen, true>
{
public:
    LLVMGetBoundarySpeciesAmountCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetBoundarySpeciesAmountCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getBoundarySpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class LLVMGetFloatingSpeciesAmountCodeGen: public
    LLVMGetValueCodeGenBase<LLVMGetFloatingSpeciesAmountCodeGen, true>
{
public:
    LLVMGetFloatingSpeciesAmountCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetFloatingSpeciesAmountCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getFloatingSpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class LLVMGetBoundarySpeciesConcentrationCodeGen: public
    LLVMGetValueCodeGenBase<LLVMGetBoundarySpeciesConcentrationCodeGen, false>
{
public:
    LLVMGetBoundarySpeciesConcentrationCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetBoundarySpeciesConcentrationCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getBoundarySpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class LLVMGetFloatingSpeciesConcentrationCodeGen: public
    LLVMGetValueCodeGenBase<LLVMGetFloatingSpeciesConcentrationCodeGen, false>
{
public:
    LLVMGetFloatingSpeciesConcentrationCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetFloatingSpeciesConcentrationCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getFloatingSpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class LLVMGetCompartmentVolumeCodeGen: public
    LLVMGetValueCodeGenBase<LLVMGetCompartmentVolumeCodeGen, false>
{
public:
    LLVMGetCompartmentVolumeCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetCompartmentVolumeCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getCompartmentIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class LLVMGetGlobalParameterCodeGen: public
    LLVMGetValueCodeGenBase<LLVMGetGlobalParameterCodeGen, false>
{
public:
    LLVMGetGlobalParameterCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetGlobalParameterCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getGlobalParameterIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};



} /* namespace rr */
#endif /* RRLLVMGETBOUNDARYSPECIESAMOUNTSCODEGEN_H_ */
