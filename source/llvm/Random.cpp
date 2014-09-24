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
#include "rrConfig.h"
#include "rrUtils.h"

using rr::Logger;
using rr::Config;
using rr::getMicroSeconds;

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

static unsigned long defaultSeed()
{
    int64_t seed = Config::getValue(Config::RANDOM_SEED).convert<int>();
    if (seed < 0)
    {
        // system time in mirsoseconds since 1970
        seed = getMicroSeconds();
    }

    unsigned long maxl = std::numeric_limits<unsigned long>::max() - 2;

    seed = seed % maxl;

    return (unsigned long)seed;
}

Random::Random(ModelGeneratorContext& ctx)
{
    addGlobalMappings(ctx);

    engine.seed(defaultSeed());
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
    Log(Logger::LOG_DEBUG) << "distrib_uniform("
            << static_cast<void*>(random)
            << ", " << _min << ", " << _max << ")";

    // gcc tr1 uniform_real is broken in that it expects
    // rng numbers to be normalized to [0,1].
    double range = random->engine.max() - random->engine.min();
    return (random->engine() / range) * (_max - _min) + _min;
}

} /* namespace rrllvm */
