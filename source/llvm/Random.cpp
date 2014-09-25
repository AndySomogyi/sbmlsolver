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
#include <stdint.h>

using rr::Logger;
using rr::Config;
using rr::getMicroSeconds;

using namespace llvm;

namespace rrllvm
{

typedef cxx11_ns::normal_distribution<double> NormalDist;

static int randomCount = 0;

/**
 * random uniform distribution
 */
static double distrib_uniform(Random *random, double _min, double _max);

static double distrib_normal(Random* random, double mu, double sigma);


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
    randomCount++;
}

Random::Random(const Random& other)
{
    *this = other;
    engine.seed(defaultSeed());
    randomCount++;
}

Random& Random::operator =(const Random& rhs)
{
    engine = rhs.engine;
    return *this;
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
    Type *int_type = Type::getInt32Ty(context);


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

    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("rr_distrib_normal",
                    FunctionType::get(double_type, args_void_double_double, false), module),
                        (void*) distrib_normal);

}

double distrib_uniform(Random *random, double _min, double _max)
{
    Log(Logger::LOG_DEBUG) << "distrib_uniform("
            << static_cast<void*>(random)
            << ", " << _min << ", " << _max << ")";

    cxx11_ns::uniform_real<double> dist(_min, _max);
    return dist(*random);
}

double distrib_normal(Random* random, double mu, double sigma)
{
    Log(Logger::LOG_DEBUG) << "distrib_normal("
            << static_cast<void*>(random)
            << ", " << mu << ", " << sigma << ")";

    NormalDist normal(mu, sigma);
    return normal(*random);
}

Random::~Random()
{
    --randomCount;
    Log(Logger::LOG_TRACE) << "deleted Random object, count: " << randomCount;
}

double Random::operator ()()
{
    double range = engine.max() - engine.min();
    return engine() / range;
}

} /* namespace rrllvm */

