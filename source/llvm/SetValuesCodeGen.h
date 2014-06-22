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

    StringIntVector getIds();

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetFloatingSpeciesAmountCodeGen: public
    SetValueCodeGenBase<SetFloatingSpeciesAmountCodeGen, true>
{
public:
    SetFloatingSpeciesAmountCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesAmountCodeGen() {};

    StringIntVector getIds();

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetBoundarySpeciesConcentrationCodeGen: public
    SetValueCodeGenBase<SetBoundarySpeciesConcentrationCodeGen, false>
{
public:
    SetBoundarySpeciesConcentrationCodeGen(const ModelGeneratorContext &mgc);
    ~SetBoundarySpeciesConcentrationCodeGen() {};

    StringIntVector getIds();

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetFloatingSpeciesConcentrationCodeGen: public
    SetValueCodeGenBase<SetFloatingSpeciesConcentrationCodeGen, false>
{
public:
    SetFloatingSpeciesConcentrationCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesConcentrationCodeGen() {};

    StringIntVector getIds();

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetCompartmentVolumeCodeGen: public
    SetValueCodeGenBase<SetCompartmentVolumeCodeGen, false>
{
public:
    SetCompartmentVolumeCodeGen(const ModelGeneratorContext &mgc);
    ~SetCompartmentVolumeCodeGen() {};

    StringIntVector getIds();

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetGlobalParameterCodeGen: public
    SetValueCodeGenBase<SetGlobalParameterCodeGen, false>
{
public:
    SetGlobalParameterCodeGen(const ModelGeneratorContext &mgc);
    ~SetGlobalParameterCodeGen() {};

    StringIntVector getIds();

    static const char* FunctionName;
    static const char* IndexArgName;
};



} /* namespace rrllvm */
#endif /* SETVALUESCODEGEN_H_ */
