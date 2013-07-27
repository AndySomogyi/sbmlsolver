/*
 * rrLLVMCodeGenBase.h
 *
 *  Created on: Jul 4, 2013
 *      Author: andy
 */

#ifndef LLVMCodeGenBaseH
#define LLVMCodeGenBaseH

#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"

namespace rr
{

/**
 * a convenience class to pull the vars out of a context, and
 * store them as ivars. It can get tedious alwasy typing mgc.getThis
 * and mgc.getThat. Furthermore, its faster to access them as ivars
 * as it does not incur a func call each time.
 */
class LLVMCodeGenBase
{

protected:

    LLVMCodeGenBase(const LLVMModelGeneratorContext &mgc) :
            model(mgc.getModel()),
            dataSymbols(mgc.getModelDataSymbols()),
            modelSymbols(mgc.getModelSymbols()),
            context(mgc.getContext()),
            module(mgc.getModule()),
            builder(mgc.getBuilder()),
            engine(mgc.getExecutionEngine())
    {
    };

    /**
     * could potentially be null, everything else is guaranteed to be valid
     */
    const libsbml::Model *model;

    const LLVMModelDataSymbols &dataSymbols;
    const LLVMModelSymbols &modelSymbols;

    llvm::LLVMContext &context;
    llvm::Module *module;
    llvm::IRBuilder<> &builder;
    llvm::ExecutionEngine &engine;


    virtual ~LLVMCodeGenBase() {};
};

} /* namespace rr */
#endif /* LLVMCodeGenBaseH */
