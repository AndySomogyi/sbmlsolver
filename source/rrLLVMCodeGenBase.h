/*
 * rrLLVMCodeGenBase.h
 *
 *  Created on: Jul 4, 2013
 *      Author: andy
 */

#ifndef LLVMCodeGenBaseH
#define LLVMCodeGenBaseH

#include "rrLLVMModelGeneratorContext.h"

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
            symbols(mgc.getModelDataSymbols()),
            model(mgc.getModel()),
            context(mgc.getContext()),
            module(mgc.getModule()),
            builder(mgc.getBuilder())
    {
    };

    const LLVMModelDataSymbols &symbols;
    libsbml::Model *model;
    llvm::LLVMContext &context;
    llvm::Module *module;
    llvm::IRBuilder<> *builder;
};

} /* namespace rr */




#endif /* LLVMCodeGenBaseH */
