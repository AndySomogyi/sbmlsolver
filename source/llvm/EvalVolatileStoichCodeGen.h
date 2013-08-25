/*
 * EvalVolatileStoichCodeGen.h
 *
 *  Created on: Aug 25, 2013
 *      Author: andy
 */

#ifndef EVALVOLATILESTOICHCODEGEN_H_
#define EVALVOLATILESTOICHCODEGEN_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include <sbml/Model.h>



namespace rrllvm
{

typedef void (*EvalVolatileStoichCodeGen_FunctionPtr)(rr::LLVMModelData*, int32_t);

class EvalVolatileStoichCodeGen:
        public rr::CodeGenBase<EvalVolatileStoichCodeGen_FunctionPtr>
{
public:
    EvalVolatileStoichCodeGen(const rr::ModelGeneratorContext &mgc);
    virtual ~EvalVolatileStoichCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
};

} /* namespace rrllvm */
#endif /* EVALVOLATILESTOICHCODEGEN_H_ */
