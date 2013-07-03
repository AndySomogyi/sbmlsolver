/*
 * rrLLVMCodeGen.h
 *
 *  Created on: Jul 2, 2013
 *      Author: andy
 */

#ifndef RRLLVMCODEGEN_H_
#define RRLLVMCODEGEN_H_

#include "rrLLVMIncludes.h"

class LLVMCodeGen
{
public:
    virtual llvm::Value *CodeGen() = 0;
protected:
    ~LLVMCodeGen() {};
};




#endif /* RRLLVMCODEGEN_H_ */
