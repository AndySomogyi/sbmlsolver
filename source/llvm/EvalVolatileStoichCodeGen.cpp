/*
 * EvalVolatileStoichCodeGen.cpp
 *
 *  Created on: Aug 25, 2013
 *      Author: andy
 */

#include "EvalVolatileStoichCodeGen.h"
#include "LLVMException.h"
#include "ASTNodeCodeGen.h"
#include "ASTNodeFactory.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>
#include <Poco/Logger.h>

namespace rrllvm
{
using namespace rr;
using namespace llvm;

const char* EvalVolatileStoichCodeGen::FunctionName = "evalVolatileStoich";

EvalVolatileStoichCodeGen::EvalVolatileStoichCodeGen(
        const ModelGeneratorContext &mgc) :
        CodeGenBase<EvalVolatileStoichCodeGen_FunctionPtr>(mgc)
{
}

EvalVolatileStoichCodeGen::~EvalVolatileStoichCodeGen()
{
}

Value* EvalVolatileStoichCodeGen::codeGen()
{
    Value *modelData = 0;

    codeGenVoidModelDataHeader(FunctionName, modelData);

    ModelDataLoadSymbolResolver resolver(modelData,model,modelSymbols,
            dataSymbols,builder);
    ModelDataIRBuilder mdbuilder(modelData, dataSymbols, builder);
    ASTNodeCodeGen astCodeGen(builder, resolver);

    builder.CreateRetVoid();

    return verifyFunction();
}

} /* namespace rrllvm */
