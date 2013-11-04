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
class SetFloatingSpeciesInitConcentrationsCodeGen: public
    SetInitialValueCodeGenBase<SetFloatingSpeciesInitConcentrationsCodeGen, false>
{
public:
    SetFloatingSpeciesInitConcentrationsCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesInitConcentrationsCodeGen() {};

    std::vector<std::pair<uint, std::string> > getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};



class SetCompartmentInitVolumesCodeGen: public
    SetInitialValueCodeGenBase<SetCompartmentInitVolumesCodeGen, false>
{
public:
    SetCompartmentInitVolumesCodeGen(const ModelGeneratorContext &mgc);
    ~SetCompartmentInitVolumesCodeGen() {};

    std::vector<std::pair<uint, std::string> > getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};


} /* namespace rrllvm */
#endif /* RRLLVMSETINITIALVALUESCODEGEN_H_ */
