/*
 * GetPiecewiseTriggersCodeGen.h
 *
 *  Created on: Aug 10, 2013
 *      Author: andy
 */

#ifndef RRLLVMGetPiecewiseTriggersCodeGen_H_
#define RRLLVMGetPiecewiseTriggersCodeGen_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeCodeGen.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include "ModelDataSymbolResolver.h"
#include "LLVMException.h"
#include "rrLogger.h"
#include <sbml/Model.h>
#include <Poco/Logger.h>
#include <vector>
#include <cstdio>

namespace rrllvm
{

    typedef double (*GetPiecewiseTriggersCodeGen_FunctionPtr)(LLVMModelData*, size_t);

    /** @class GetPiecewiseTriggersCodeGen
    * Class for getting piecewise trigger values.
    */
    class GetPiecewiseTriggersCodeGen : public
        CodeGenBase<GetPiecewiseTriggersCodeGen>
    {
    public:
        GetPiecewiseTriggersCodeGen(const ModelGeneratorContext& mgc) :
            CodeGenBase<GetPiecewiseTriggersCodeGen_FunctionPtr>(mgc)
        {
        };

        virtual ~GetPiecewiseTriggersCodeGen() {};

        llvm::Value* codeGen();

        typedef GetPiecewiseTriggersCodeGen_FunctionPtr FunctionPtr;

        static const char* FunctionName;
        static const char* IndexArgName;

        llvm::Type* getRetType();

        llvm::Value* createRet(llvm::Value*);
    };


} /* namespace rr */




#endif /* RRLLVMGETVALUECODEGENBASE_H_ */
