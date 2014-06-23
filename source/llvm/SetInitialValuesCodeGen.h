/*
 * GetValuesCodeGen.h
 *
 *  Created on: Sept 16, 2013
 *      Author: andy
 */

#ifndef RRLLVMSETINITIALVALUESCODEGEN_H_
#define RRLLVMSETINITIALVALUESCODEGEN_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include "SetInitialValueCodeGenBase.h"
#include <sbml/Model.h>

namespace rrllvm
{
class SetFloatingSpeciesInitConcentrationCodeGen: public
    SetInitialValueCodeGenBase<SetFloatingSpeciesInitConcentrationCodeGen, false>
{
public:
    SetFloatingSpeciesInitConcentrationCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesInitConcentrationCodeGen() {};

    StringIntVector getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};



class SetFloatingSpeciesInitAmountCodeGen: public
    SetInitialValueCodeGenBase<SetFloatingSpeciesInitAmountCodeGen, true>
{
public:
    SetFloatingSpeciesInitAmountCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesInitAmountCodeGen() {};

    StringIntVector getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};



class SetCompartmentInitVolumeCodeGen: public
    SetInitialValueCodeGenBase<SetCompartmentInitVolumeCodeGen, false>
{
public:
    SetCompartmentInitVolumeCodeGen(const ModelGeneratorContext &mgc);
    ~SetCompartmentInitVolumeCodeGen() {};

    StringIntVector getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class SetGlobalParameterInitValueCodeGen: public
    SetInitialValueCodeGenBase<SetGlobalParameterInitValueCodeGen, false>
{
public:
    SetGlobalParameterInitValueCodeGen(const ModelGeneratorContext &mgc);
    ~SetGlobalParameterInitValueCodeGen() {};

    StringIntVector getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};


} /* namespace rrllvm */
#endif /* RRLLVMSETINITIALVALUESCODEGEN_H_ */
