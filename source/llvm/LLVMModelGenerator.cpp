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

using llvm::Function;
using llvm::ExecutionEngine;

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
    Log(Logger::LOG_TRACE) << __FUNC__;
}

LLVMModelGenerator::~LLVMModelGenerator()
{
    Log(Logger::LOG_TRACE) << __FUNC__;

}

bool LLVMModelGenerator::setTemporaryDirectory(const string& path)
{
    return true;
}

string LLVMModelGenerator::getTemporaryDirectory()
{
    return "not used";
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
            options & ModelGenerator::CONSERVED_MOIETIES;

    bool forceReCompile = options & ModelGenerator::RECOMPILE;

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
            Log(Logger::LOG_DEBUG) << "found a cached model for " << md5;
            return new LLVMExecutableModel(sp, createModelData(*sp->symbols));
        }
        else
        {
            Log(Logger::LOG_TRACE) << "no cached model found for " << md5
                    << ", creating new one";
        }
    }

    SharedModelPtr rc(new ModelResources());

    ModelGeneratorContext context(sbml, options);

    Function* evalInitialConditions = 0;
    Function* evalReactionRates = 0;
    Function* getBoundarySpeciesAmount = 0;
    Function* getFloatingSpeciesAmount = 0;
    Function* getBoundarySpeciesConcentration = 0;
    Function* getFloatingSpeciesConcentration = 0;
    Function* getCompartmentVolume = 0;
    Function* getGlobalParameter = 0;
    Function* evalRateRuleRates = 0;
    Function* getEventTrigger = 0;
    Function* getEventPriority = 0;
    Function* getEventDelay = 0;
    Function* eventTrigger = 0;
    Function* eventAssign = 0;
    Function* evalVolatileStoich = 0;
    Function* evalConversionFactor = 0;
    Function* setBoundarySpeciesAmount = 0;
    Function* setBoundarySpeciesConcentration = 0;
    Function* setFloatingSpeciesConcentration = 0;
    Function* setCompartmentVolume = 0;
    Function* setFloatingSpeciesAmount = 0;
    Function* setGlobalParameter = 0;
    Function* getFloatingSpeciesInitConcentrations = 0;
    Function* setFloatingSpeciesInitConcentrations = 0;
    Function* getFloatingSpeciesInitAmounts = 0;
    Function* setFloatingSpeciesInitAmounts = 0;
    Function* getCompartmentInitVolumes = 0;
    Function* setCompartmentInitVolumes = 0;

    evalInitialConditions =
        EvalInitialConditionsCodeGen(context).createFunction();

    evalReactionRates =
        EvalReactionRatesCodeGen(context).createFunction();

    getBoundarySpeciesAmount =
        GetBoundarySpeciesAmountCodeGen(context).createFunction();

    getFloatingSpeciesAmount =
        GetFloatingSpeciesAmountCodeGen(context).createFunction();

    getBoundarySpeciesConcentration =
        GetBoundarySpeciesConcentrationCodeGen(context).createFunction();

    getFloatingSpeciesConcentration =
        GetFloatingSpeciesConcentrationCodeGen(context).createFunction();

    getCompartmentVolume =
        GetCompartmentVolumeCodeGen(context).createFunction();

    getGlobalParameter =
        GetGlobalParameterCodeGen(context).createFunction();

    evalRateRuleRates =
        EvalRateRuleRatesCodeGen(context).createFunction();

    getEventTrigger =
        GetEventTriggerCodeGen(context).createFunction();

    getEventPriority =
        GetEventPriorityCodeGen(context).createFunction();

    getEventDelay =
        GetEventDelayCodeGen(context).createFunction();

    eventTrigger =
        EventTriggerCodeGen(context).createFunction();

    eventAssign =
        EventAssignCodeGen(context).createFunction();

    evalVolatileStoich =
        EvalVolatileStoichCodeGen(context).createFunction();

    evalConversionFactor =
        EvalConversionFactorCodeGen(context).createFunction();

    if (!(options & ModelGenerator::READ_ONLY))
    {
        setBoundarySpeciesAmount =
            SetBoundarySpeciesAmountCodeGen(context).createFunction();

        setBoundarySpeciesConcentration =
            SetBoundarySpeciesConcentrationCodeGen(context).createFunction();

        setFloatingSpeciesConcentration =
            SetFloatingSpeciesConcentrationCodeGen(context).createFunction();

        setCompartmentVolume =
            SetCompartmentVolumeCodeGen(context).createFunction();

        setFloatingSpeciesAmount =
            SetFloatingSpeciesAmountCodeGen(context).createFunction();

        setGlobalParameter =
            SetGlobalParameterCodeGen(context).createFunction();
    }

    if (options & ModelGenerator::MUTABLE_INITIAL_CONDITIONS)
    {
        getFloatingSpeciesInitConcentrations =
            GetFloatingSpeciesInitConcentrationCodeGen(context).createFunction();

        setFloatingSpeciesInitConcentrations =
            SetFloatingSpeciesInitConcentrationCodeGen(context).createFunction();

        getFloatingSpeciesInitAmounts =
            GetFloatingSpeciesInitAmountCodeGen(context).createFunction();

        setFloatingSpeciesInitAmounts =
            SetFloatingSpeciesInitAmountCodeGen(context).createFunction();

        getCompartmentInitVolumes =
            GetCompartmentInitVolumeCodeGen(context).createFunction();

        setCompartmentInitVolumes =
            SetCompartmentInitVolumeCodeGen(context).createFunction();
    }

    ExecutionEngine& engine = context.getExecutionEngine();

    // if MCJIT, this actually JITs all the code, if regular JIT, does nothing
    engine.finalizeObject();

    rc->evalInitialConditionsPtr =
        EvalInitialConditionsCodeGen::getPointerToFunction(engine, evalInitialConditions);

    rc->evalReactionRatesPtr =
        EvalReactionRatesCodeGen::getPointerToFunction(engine, evalReactionRates);

    rc->getBoundarySpeciesAmountPtr =
        GetBoundarySpeciesAmountCodeGen::getPointerToFunction(engine, getBoundarySpeciesAmount);

    rc->getFloatingSpeciesAmountPtr =
        GetFloatingSpeciesAmountCodeGen::getPointerToFunction(engine, getFloatingSpeciesAmount);

    rc->getBoundarySpeciesConcentrationPtr =
        GetBoundarySpeciesConcentrationCodeGen::getPointerToFunction(engine, getBoundarySpeciesConcentration);

    rc->getFloatingSpeciesConcentrationPtr =
        GetFloatingSpeciesConcentrationCodeGen::getPointerToFunction(engine, getFloatingSpeciesConcentration);

    rc->getCompartmentVolumePtr =
        GetCompartmentVolumeCodeGen::getPointerToFunction(engine, getCompartmentVolume);

    rc->getGlobalParameterPtr =
        GetGlobalParameterCodeGen::getPointerToFunction(engine, getGlobalParameter);

    rc->evalRateRuleRatesPtr =
        EvalRateRuleRatesCodeGen::getPointerToFunction(engine, evalRateRuleRates);

    rc->getEventTriggerPtr =
        GetEventTriggerCodeGen::getPointerToFunction(engine, getEventTrigger);

    rc->getEventPriorityPtr =
        GetEventPriorityCodeGen::getPointerToFunction(engine, getEventPriority);

    rc->getEventDelayPtr =
        GetEventDelayCodeGen::getPointerToFunction(engine, getEventDelay);

    rc->eventTriggerPtr =
        EventTriggerCodeGen::getPointerToFunction(engine, eventTrigger);

    rc->eventAssignPtr =
        EventAssignCodeGen::getPointerToFunction(engine, eventAssign);

    rc->evalVolatileStoichPtr =
        EvalVolatileStoichCodeGen::getPointerToFunction(engine, evalVolatileStoich);

    rc->evalConversionFactorPtr =
        EvalConversionFactorCodeGen::getPointerToFunction(engine, evalConversionFactor);

    if (!(options & ModelGenerator::READ_ONLY))
    {
        rc->setBoundarySpeciesAmountPtr =
            SetBoundarySpeciesAmountCodeGen::getPointerToFunction(engine, setBoundarySpeciesAmount);

        rc->setBoundarySpeciesConcentrationPtr =
            SetBoundarySpeciesConcentrationCodeGen::getPointerToFunction(engine, setBoundarySpeciesConcentration);

        rc->setFloatingSpeciesConcentrationPtr =
            SetFloatingSpeciesConcentrationCodeGen::getPointerToFunction(engine, setFloatingSpeciesConcentration);

        rc->setCompartmentVolumePtr =
            SetCompartmentVolumeCodeGen::getPointerToFunction(engine, setCompartmentVolume);

        rc->setFloatingSpeciesAmountPtr =
            SetFloatingSpeciesAmountCodeGen::getPointerToFunction(engine, setFloatingSpeciesAmount);

        rc->setGlobalParameterPtr =
            SetGlobalParameterCodeGen::getPointerToFunction(engine, setGlobalParameter);
    }

    if (options & ModelGenerator::MUTABLE_INITIAL_CONDITIONS)
    {
        rc->getFloatingSpeciesInitConcentrationsPtr =
            GetFloatingSpeciesInitConcentrationCodeGen::getPointerToFunction(engine, getFloatingSpeciesInitConcentrations);

        rc->setFloatingSpeciesInitConcentrationsPtr =
            SetFloatingSpeciesInitConcentrationCodeGen::getPointerToFunction(engine, setFloatingSpeciesInitConcentrations);

        rc->getFloatingSpeciesInitAmountsPtr =
            GetFloatingSpeciesInitAmountCodeGen::getPointerToFunction(engine, getFloatingSpeciesInitAmounts);

        rc->setFloatingSpeciesInitAmountsPtr =
            SetFloatingSpeciesInitAmountCodeGen::getPointerToFunction(engine, setFloatingSpeciesInitAmounts);

        rc->getCompartmentInitVolumesPtr =
            GetCompartmentInitVolumeCodeGen::getPointerToFunction(engine, getCompartmentInitVolumes);

        rc->setCompartmentInitVolumesPtr =
            SetCompartmentInitVolumeCodeGen::getPointerToFunction(engine, setCompartmentInitVolumes);
    }


    // if anything up to this point throws an exception, thats OK, because
    // we have not allocated any memory yet that is not taken care of by
    // something else.
    // Now that everything that could have thrown would have thrown, we
    // can now create the model and set its fields.

    LLVMModelData *modelData = createModelData(context.getModelDataSymbols());

    uint llvmsize = ModelDataIRBuilder::getModelDataSize(context.getModule(),
            &context.getExecutionEngine());

    if (llvmsize != modelData->size)
    {
        std::stringstream s;

        s << "LLVM Model Data size " << llvmsize << " is different from " <<
                "C++ size of LLVM ModelData, " << modelData->size;

        LLVMModelData_free(modelData);

        Log(Logger::LOG_FATAL) << s.str();

        throw_llvm_exception(s.str());
    }

    // * MOVE * the bits over from the context to the exe model.
    context.stealThePeach(&rc->symbols, &rc->context,
            &rc->executionEngine, &rc->errStr);

    rc->useMCJIT = context.useMCJIT();

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
                Log(Logger::LOG_DEBUG) <<
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
            Log(Logger::LOG_DEBUG) << "could not find existing cached resource "
                    "resources, for hash " << md5 <<
                    ", inserting new resources into cache";

            cachedModels[md5] = rc;
        }

        cachedModelsMutex.unlock();
    }

    return new LLVMExecutableModel(rc, modelData);
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

LLVMModelData *createModelData(const rrllvm::LLVMModelDataSymbols &symbols)
{
    uint modelDataBaseSize = sizeof(LLVMModelData);

    uint numIndCompartments = symbols.getIndependentCompartmentSize();
    uint numIndFloatingSpecies = symbols.getIndependentFloatingSpeciesSize();
    uint numConservedSpecies = symbols.getConservedSpeciesSize();
    uint numIndBoundarySpecies = symbols.getIndependentBoundarySpeciesSize();
    uint numIndGlobalParameters = symbols.getIndependentGlobalParameterSize();

    uint numInitCompartments = symbols.getInitCompartmentSize();
    uint numInitFloatingSpecies = symbols.getInitFloatingSpeciesSize();
    uint numInitBoundarySpecies = symbols.getInitBoundarySpeciesSize();
    uint numInitGlobalParameters = symbols.getInitGlobalParameterSize();

    // no initial conditions for these
    uint numRateRules = symbols.getRateRuleSize();
    uint numReactions = symbols.getReactionSize();

    uint modelDataSize = modelDataBaseSize +
        sizeof(double) * (
            numIndCompartments +
            numInitCompartments +
            numInitFloatingSpecies +
            numConservedSpecies +
            numIndBoundarySpecies +
            numInitBoundarySpecies +
            numIndGlobalParameters +
            numInitGlobalParameters +
            numReactions +
            numRateRules +
            numIndFloatingSpecies
            );

    LLVMModelData *modelData = (LLVMModelData*)calloc(
            modelDataSize, sizeof(unsigned char));

    modelData->size = modelDataSize;
    modelData->numIndCompartments = numIndCompartments;
    modelData->numIndFloatingSpecies = numIndFloatingSpecies;
    modelData->numIndBoundarySpecies = numIndBoundarySpecies;
    modelData->numConservedSpecies = numConservedSpecies;
    modelData->numIndGlobalParameters = numIndGlobalParameters;

    modelData->numInitCompartments = numInitCompartments;
    modelData->numInitFloatingSpecies = numInitFloatingSpecies;
    modelData->numInitBoundarySpecies = numInitBoundarySpecies;
    modelData->numInitBoundarySpecies = numInitGlobalParameters;

    modelData->numRateRules = numRateRules;
    modelData->numReactions = numReactions;
    modelData->numEvents = symbols.getEventAttributes().size();

    // set the aliases to the offsets
    uint offset = 0;

    modelData->compartmentVolumesAlias = &modelData->data[offset];
    offset += numIndCompartments;

    modelData->initCompartmentVolumesAlias = &modelData->data[offset];
    offset += numInitCompartments;

    modelData->initFloatingSpeciesAmountsAlias = &modelData->data[offset];
    offset += numInitFloatingSpecies;

    modelData->initConservedSpeciesAmountsAlias = &modelData->data[offset];
    offset += numConservedSpecies;

    modelData->boundarySpeciesAmountsAlias = &modelData->data[offset];
    offset += numIndBoundarySpecies;

    modelData->initBoundarySpeciesAmountsAlias = &modelData->data[offset];
    offset += numInitBoundarySpecies;

    modelData->globalParametersAlias = &modelData->data[offset];
    offset += numIndGlobalParameters;

    modelData->initGlobalParametersAlias = &modelData->data[offset];
    offset += numInitGlobalParameters;

    modelData->reactionRatesAlias = &modelData->data[offset];
    offset += numReactions;

    modelData->rateRuleValuesAlias = &modelData->data[offset];
    offset += numRateRules;

    modelData->floatingSpeciesAmountsAlias = &modelData->data[offset];
    offset += numIndFloatingSpecies;

    assert (modelDataBaseSize + offset * sizeof(double) == modelDataSize  &&
            "LLVMModelData size not equal to base size + data");

    // allocate the stoichiometry matrix
    const std::vector<uint> &stoichRowIndx = symbols.getStoichRowIndx();
    const std::vector<uint> &stoichColIndx = symbols.getStoichColIndx();
    std::vector<double> stoichValues(stoichRowIndx.size(), 0);

    modelData->stoichiometry = rr::csr_matrix_new(numIndFloatingSpecies, numReactions,
            stoichRowIndx, stoichColIndx, stoichValues);

    return modelData;
}

} /* namespace rrllvm */


