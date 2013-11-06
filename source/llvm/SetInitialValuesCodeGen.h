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

    std::vector<std::pair<uint, std::string> > getIds();

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

    std::vector<std::pair<uint, std::string> > getIds();

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

    std::vector<std::pair<uint, std::string> > getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};


} /* namespace rrllvm */
#endif /* RRLLVMSETINITIALVALUESCODEGEN_H_ */
