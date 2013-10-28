/*
 * LLVMModelGenerator.cpp
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#pragma hdrstop
#include "LLVMModelGenerator.h"
#include "LLVMExecutableModel.h"
#include "ModelGeneratorContext.h"
#include "LLVMIncludes.h"
#include "ModelResources.h"
#include "rrUtils.h"
#include <rrLogger.h>
#include <Poco/Mutex.h>

using rr::Logger;
using rr::getLogger;
using rr::ExecutableModel;
using rr::ModelGenerator;
using rr::Compiler;

namespace rrllvm
{

typedef std::tr1::weak_ptr<ModelResources> WeakModelPtr;
typedef std::tr1::shared_ptr<ModelResources> SharedModelPtr;
typedef std::tr1::unordered_map<std::string, WeakModelPtr> ModelPtrMap;

static Poco::Mutex cachedModelsMutex;
static ModelPtrMap cachedModels;

/**
 * copy the cached model fields between a cached model, and a
 * executable model.
 *
 * We don't want to have ExecutableModel inherit from CahcedModel
 * because they do compleltly different things, and have completly
 * differnt deletion semantics
 */
template <typename a_type, typename b_type>
void copyCachedModel(a_type* src, b_type* dst)
{
    dst->symbols = src->symbols;
    dst->context = src->context;
    dst->executionEngine = src->executionEngine;
    dst->errStr = src->errStr;

    dst->evalInitialConditionsPtr = src->evalInitialConditionsPtr;
    dst->evalReactionRatesPtr = src->evalReactionRatesPtr;
    dst->getBoundarySpeciesAmountPtr = src->getBoundarySpeciesAmountPtr;
    dst->getFloatingSpeciesAmountPtr = src->getFloatingSpeciesAmountPtr;
    dst->getBoundarySpeciesConcentrationPtr = src->getBoundarySpeciesConcentrationPtr;
    dst->getFloatingSpeciesConcentrationPtr = src->getFloatingSpeciesConcentrationPtr;
    dst->getCompartmentVolumePtr = src->getCompartmentVolumePtr;
    dst->getGlobalParameterPtr = src->getGlobalParameterPtr;
    dst->evalRateRuleRatesPtr = src->evalRateRuleRatesPtr;
    dst->getEventTriggerPtr = src->getEventTriggerPtr;
    dst->getEventPriorityPtr = src->getEventPriorityPtr;
    dst->getEventDelayPtr = src->getEventDelayPtr;
    dst->eventTriggerPtr = src->eventTriggerPtr;
    dst->eventAssignPtr = src->eventAssignPtr;
    dst->evalVolatileStoichPtr = src->evalVolatileStoichPtr;
    dst->evalConversionFactorPtr = src->evalConversionFactorPtr;
}


LLVMModelGenerator::LLVMModelGenerator()
{
    Log(Logger::TRACE) << __FUNC__;
}

LLVMModelGenerator::~LLVMModelGenerator()
{
    Log(Logger::TRACE) << __FUNC__;

}

bool LLVMModelGenerator::setTemporaryDirectory(const string& path)
{
    return true;
}

string LLVMModelGenerator::getTemporaryDirectory()
{
    return LLVMCompiler::gurgle();
}

class test
{
public:
    const int* p;
};

void testt(const int** p)
{
    *p = 0;
}

void testtt()
{
    test *t = new test();

    testt(&t->p);
}




ExecutableModel* LLVMModelGenerator::createModel(const std::string& sbml,
        uint options)
{
    bool computeAndAssignConsevationLaws =
            options & ModelGenerator::ComputeAndAssignConsevationLaws;

    bool forceReCompile = options & ModelGenerator::ForceReCompile;

    string md5;

    if (!forceReCompile)
    {
        // check for a chached copy
        md5 = rr::getMD5(sbml);

        ModelPtrMap::const_iterator i;

        SharedModelPtr sp;

        cachedModelsMutex.lock();

        if ((i = cachedModels.find(md5)) != cachedModels.end())
        {
            sp = i->second.lock();
        }

        cachedModelsMutex.unlock();

        // we could have recieved a bad ptr, a model could have been deleted,
        // in which case, we should have a bad ptr.

        if (sp)
        {
            Log(Logger::DEBUG) << "found a cached model for " << md5;
            return new LLVMExecutableModel(sp);
        }
        else
        {
            Log(Logger::TRACE) << "no cached model found for " << md5
                    << ", creating new one";
        }
    }

    SharedModelPtr rc(new ModelResources());

    ModelGeneratorContext context(sbml, computeAndAssignConsevationLaws);

    rc->evalInitialConditionsPtr =
            EvalInitialConditionsCodeGen(context).createFunction();

    rc->evalReactionRatesPtr =
            EvalReactionRatesCodeGen(context).createFunction();

    rc->getBoundarySpeciesAmountPtr =
            GetBoundarySpeciesAmountCodeGen(context).createFunction();

    rc->getFloatingSpeciesAmountPtr =
            GetFloatingSpeciesAmountCodeGen(context).createFunction();

    rc->getBoundarySpeciesConcentrationPtr =
            GetBoundarySpeciesConcentrationCodeGen(context).createFunction();

    rc->getFloatingSpeciesConcentrationPtr =
            GetFloatingSpeciesConcentrationCodeGen(context).createFunction();

    rc->getCompartmentVolumePtr =
            GetCompartmentVolumeCodeGen(context).createFunction();

    rc->getGlobalParameterPtr =
            GetGlobalParameterCodeGen(context).createFunction();

    rc->evalRateRuleRatesPtr =
            EvalRateRuleRatesCodeGen(context).createFunction();

    rc->getEventTriggerPtr =
            GetEventTriggerCodeGen(context).createFunction();

    rc->getEventPriorityPtr =
            GetEventPriorityCodeGen(context).createFunction();

    rc->getEventDelayPtr =
            GetEventDelayCodeGen(context).createFunction();

    rc->eventTriggerPtr =
            EventTriggerCodeGen(context).createFunction();

    rc->eventAssignPtr =
            EventAssignCodeGen(context).createFunction();

    rc->evalVolatileStoichPtr =
            EvalVolatileStoichCodeGen(context).createFunction();

    rc->evalConversionFactorPtr =
            EvalConversionFactorCodeGen(context).createFunction();

    if (options & ModelGenerator::ReadOnlyModel)
    {
        rc->setBoundarySpeciesAmountPtr = 0;
        rc->setBoundarySpeciesConcentrationPtr = 0;
        rc->setFloatingSpeciesConcentrationPtr = 0;
        rc->setCompartmentVolumePtr = 0;
        rc->setFloatingSpeciesAmountPtr = 0;
        rc->setGlobalParameterPtr = 0;
    }
    else
    {
        rc->setBoundarySpeciesAmountPtr = SetBoundarySpeciesAmountCodeGen(
                context).createFunction();

        rc->setBoundarySpeciesConcentrationPtr =
                SetBoundarySpeciesConcentrationCodeGen(context).createFunction();

        rc->setFloatingSpeciesConcentrationPtr =
                SetFloatingSpeciesConcentrationCodeGen(context).createFunction();

        rc->setCompartmentVolumePtr =
                SetCompartmentVolumeCodeGen(context).createFunction();

        rc->setFloatingSpeciesAmountPtr = SetFloatingSpeciesAmountCodeGen(
                context).createFunction();

        rc->setGlobalParameterPtr =
                SetGlobalParameterCodeGen(context).createFunction();
    }


    // if anything up to this point throws an exception, thats OK, because
    // we have not allocated any memory yet that is not taken care of by
    // something else.
    // Now that everything that could have thrown would have thrown, we
    // can now create the model and set its fields.

    // * MOVE * the bits over from the context to the exe model.
    context.stealThePeach(&rc->symbols, &rc->context,
            &rc->executionEngine, &rc->errStr);

    if (!forceReCompile)
    {
        // check for a chached copy, another thread could have
        // created one while we were making ours...

        ModelPtrMap::const_iterator i;

        SharedModelPtr sp;

        cachedModelsMutex.lock();

        // whilst we have it locked, clear any expired ptrs
        for (ModelPtrMap::const_iterator j = cachedModels.begin();
                j != cachedModels.end();)
        {
            if (j->second.expired())
            {
                Log(Logger::INFORMATION) <<
                        "removing expired model resource for hash " << md5;

                j = cachedModels.erase(j);
            }
            else
            {
                ++j;
            }
        }

        if ((i = cachedModels.find(md5)) == cachedModels.end())
        {
            Log(Logger::INFORMATION) << "could not find existing cached resource "
                    "resources, for hash " << md5 <<
                    ", inserting new resources into cache";

            cachedModels[md5] = rc;
        }

        cachedModelsMutex.unlock();
    }

    return new LLVMExecutableModel(rc);
}

Compiler* LLVMModelGenerator::getCompiler()
{
    return &compiler;
}

bool LLVMModelGenerator::setCompiler(const string& compiler)
{
    return true;
}

/************ LLVM Utility Functions, TODO: Move To Separate File ************/

/**
 * C++ 11 style to_string for LLVM types
 */
std::string to_string(const llvm::Value *value)
{
    std::string str;
    llvm::raw_string_ostream stream(str);
    value->print(stream);
    return str;
}

} /* namespace rr */


