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
class SetFloatingSpeciesInitialConcentrationCodeGen: public
    SetInitialValueCodeGenBase<SetFloatingSpeciesInitialConcentrationCodeGen, false>
{
public:
    SetFloatingSpeciesInitialConcentrationCodeGen(const ModelGeneratorContext &mgc);
    ~SetFloatingSpeciesInitialConcentrationCodeGen() {};

    std::vector<std::pair<uint, std::string> > getIds();

    bool isInitialValue() {
        return true;
    }

    static const char* FunctionName;
    static const char* IndexArgName;
};


} /* namespace rrllvm */
#endif /* RRLLVMSETINITIALVALUESCODEGEN_H_ */
