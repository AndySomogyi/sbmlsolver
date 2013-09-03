/*
 * EvalConversionFactorCodeGen.h
 *
 *  Created on: Aug 25, 2013
 *      Author: andy
 */

#ifndef EVALCONVERSIONFACTORCODEGEN_H_
#define EVALCONVERSIONFACTORCODEGEN_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include <sbml/Model.h>

namespace rrllvm
{

typedef void (*EvalConversionFactorCodeGen_FunctionPtr)(LLVMModelData*);

class EvalConversionFactorCodeGen:
        public CodeGenBase<EvalConversionFactorCodeGen_FunctionPtr>
{
public:
    EvalConversionFactorCodeGen(const ModelGeneratorContext &mgc);
    virtual ~EvalConversionFactorCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;

};

} /* namespace rrllvm */
#endif /* EVALCONVERSIONFACTORCODEGEN_H_ */
