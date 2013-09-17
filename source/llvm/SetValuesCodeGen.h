/*
 * GetValuesCodeGen.h
 *
 *  Created on: Sept 16, 2013
 *      Author: andy
 */

#ifndef RRLLVMSETVALUESCODEGEN_H_
#define RRLLVMSETVALUESCODEGEN_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include "SetValueCodeGenBase.h"
#include <sbml/Model.h>

namespace rrllvm
{

class SetBoundarySpeciesAmountCodeGen: public
    SetValueCodeGenBase<SetBoundarySpeciesAmountCodeGen, true>
{
public:
    SetBoundarySpeciesAmountCodeGen(const ModelGeneratorContext &mgc);
    ~SetBoundarySpeciesAmountCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getBoundarySpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetFloatingSpeciesAmountCodeGen: public
    SetValueCodeGenBase<SetFloatingSpeciesAmountCodeGen, true>
{
public:
    SetFloatingSpeciesAmountCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesAmountCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getFloatingSpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetBoundarySpeciesConcentrationCodeGen: public
    SetValueCodeGenBase<SetBoundarySpeciesConcentrationCodeGen, false>
{
public:
    SetBoundarySpeciesConcentrationCodeGen(const ModelGeneratorContext &mgc);
    ~SetBoundarySpeciesConcentrationCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getBoundarySpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetFloatingSpeciesConcentrationCodeGen: public
    SetValueCodeGenBase<SetFloatingSpeciesConcentrationCodeGen, false>
{
public:
    SetFloatingSpeciesConcentrationCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesConcentrationCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getFloatingSpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetCompartmentVolumeCodeGen: public
    SetValueCodeGenBase<SetCompartmentVolumeCodeGen, false>
{
public:
    SetCompartmentVolumeCodeGen(const ModelGeneratorContext &mgc);
    ~SetCompartmentVolumeCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getCompartmentIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetGlobalParameterCodeGen: public
    SetValueCodeGenBase<SetGlobalParameterCodeGen, false>
{
public:
    SetGlobalParameterCodeGen(const ModelGeneratorContext &mgc);
    ~SetGlobalParameterCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getGlobalParameterIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};



} /* namespace rrllvm */
#endif /* SETVALUESCODEGEN_H_ */
