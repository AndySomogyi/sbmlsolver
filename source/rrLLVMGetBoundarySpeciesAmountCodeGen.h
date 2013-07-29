/*
 * rrLLVMGetBoundarySpeciesAmountsCodeGen.h
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */

#ifndef RRLLVMGETBOUNDARYSPECIESAMOUNTSCODEGEN_H_
#define RRLLVMGETBOUNDARYSPECIESAMOUNTSCODEGEN_H_

#include <rrLLVMCodeGenBase.h>
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include <sbml/Model.h>

namespace rr
{

class LLVMGetBoundarySpeciesAmountCodeGen: public rr::LLVMCodeGenBase
{
public:
    LLVMGetBoundarySpeciesAmountCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetBoundarySpeciesAmountCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef double (*FunctionPtr)(ModelData*, int32_t);

    FunctionPtr createFunction();

private:
    llvm::Function *functionValue;

};

} /* namespace rr */
#endif /* RRLLVMGETBOUNDARYSPECIESAMOUNTSCODEGEN_H_ */
