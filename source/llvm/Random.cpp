/*
 * Random.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: andy
 */

#include "Random.h"
#include "ModelGeneratorContext.h"
#include "LLVMIncludes.h"
#include "rrLogger.h"

using rr::Logger;

using namespace llvm;

namespace rrllvm
{

/**
 * random uniform distribution
 */
static double distrib_uniform(Random *random, double _min, double _max);


static Function* createGlobalMappingFunction(const char* funcName,
        llvm::FunctionType *funcType, Module *module);

static void addGlobalMappings(const ModelGeneratorContext& ctx);

Random::Random(ModelGeneratorContext& ctx)
{
    addGlobalMappings(ctx);
}


Function* createGlobalMappingFunction(const char* funcName,
        llvm::FunctionType *funcType, Module *module)
{
    return Function::Create(funcType, Function::InternalLinkage, funcName, module);
}

void addGlobalMappings(const ModelGeneratorContext& ctx)
{
    llvm::Module *module = ctx.getModule();
    LLVMContext& context = module->getContext();
    llvm::ExecutionEngine *executionEngine = &ctx.getExecutionEngine();
    Type *double_type = Type::getDoubleTy(context);


    // LLVM does not appear to have a true void ptr, so just use a pointer
    // to a byte, pointers are all the same size anyway.
    // used for the LLVMModelData::random which is not accessed by
    // generated llvm code anyway.
    // see also, llvm::StructType *ModelDataIRBuilder::createModelDataStructType(...)
    Type *voidPtrType = Type::getInt8PtrTy(context);

    Type* args_void_double_double[] = { voidPtrType, double_type, double_type };


    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("rr_distrib_uniform",
                    FunctionType::get(double_type, args_void_double_double, false), module),
                        (void*) distrib_uniform);

}

double distrib_uniform(Random *random, double _min, double _max)
{
    Log(Logger::LOG_NOTICE) << "distrib_uniform("
            << static_cast<void*>(random)
            << ", " << _min << ", " << _max << ")";

    cxx11_ns::uniform_real<double> dist(_min, _max);
    return dist(random->engine);
}

} /* namespace rrllvm */
