/*
 * GetValuesCodeGen.h
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */

#ifndef RRLLVM_GETINITIALVALUESCODEGEN_H_
#define RRLLVM_GETINITIALVALUESCODEGEN_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include "GetInitialValueCodeGenBase.h"
#include <sbml/Model.h>

namespace rrllvm
{



class GetFloatingSpeciesInitConcentrationCodeGen: public
    GetInitialValueCodeGenBase<GetFloatingSpeciesInitConcentrationCodeGen, false>
{
public:
    GetFloatingSpeciesInitConcentrationCodeGen(const ModelGeneratorContext &mgc);
    ~GetFloatingSpeciesInitConcentrationCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getFloatingSpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class GetFloatingSpeciesInitAmountCodeGen: public
    GetInitialValueCodeGenBase<GetFloatingSpeciesInitAmountCodeGen, true>
{
public:
    GetFloatingSpeciesInitAmountCodeGen(const ModelGeneratorContext &mgc);
    ~GetFloatingSpeciesInitAmountCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getFloatingSpeciesIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};

class GetCompartmentInitVolumeCodeGen: public
    GetInitialValueCodeGenBase<GetCompartmentInitVolumeCodeGen, false>
{
public:
    GetCompartmentInitVolumeCodeGen(const ModelGeneratorContext &mgc);
    ~GetCompartmentInitVolumeCodeGen() {};

    std::vector<string> getIds()
    {
        return dataSymbols.getCompartmentIds();
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};



} /* namespace rr */
#endif /* RRLLVM_GETINITIALVALUESCODEGEN_H_ */
