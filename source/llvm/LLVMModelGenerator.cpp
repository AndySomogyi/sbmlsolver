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

#define NOMINMAX

#include "LLVMModelGenerator.h"
#include "LLVMExecutableModel.h"
#include "ModelGeneratorContext.h"
#include "LLVMIncludes.h"
#include "ModelResources.h"
#include "Random.h"
#include <rrLogger.h>
#include <rrUtils.h>
#include <Poco/Mutex.h>

#ifdef _MSC_VER
#pragma warning(disable: 4146)
#pragma warning(disable: 4141)
#pragma warning(disable: 4267)
#pragma warning(disable: 4624)
#endif


#include "llvm/Object/ObjectFile.h"
#include "llvm/LLVMExecutableModel.h"

#include "llvm/MCJit.h"

#if LLVM_VERSION_MAJOR >= 13

#   include "llvm/rrLLJit.h"

#endif

#ifdef _MSC_VER
#pragma warning(default: 4146)
#pragma warning(default: 4141)
#pragma warning(default: 4267)
#pragma warning(default: 4624)
#endif


using rr::Logger;
using rr::getLogger;
using rr::ExecutableModel;
using rr::LoadSBMLOptions;
using rr::Compiler;
using llvm::Function;

namespace rrllvm {

    typedef std::weak_ptr<ModelResources> WeakModelResourcesPtr;
    typedef std::shared_ptr<ModelResources> SharedModelResourcesPtr;
    typedef std::unordered_map<std::string, WeakModelResourcesPtr> ModelResourcesPtrMap;

    static Poco::Mutex cachedModelsMutex;
    static ModelResourcesPtrMap cachedModelResources;


/**
 * copy the cached model fields between a cached model, and a
 * executable model.
 *
 * We don't want to have ExecutableModel inherit from CahcedModel
 * because they do compleltly different things, and have completly
 * differnt deletion semantics
 */
    template<typename a_type, typename b_type>
    void copyCachedModel(a_type *src, b_type *dst) {
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

///**
// * @brief cross platform mechanism for getting the target machine
// * file type.
// */
//#if LLVM_VERSION_MAJOR == 6
//    llvm::LLVMTargetMachine::CodeGenFileType getCodeGenFileType(){
//        return llvm::TargetMachine::CGFT_ObjectFile;
//    }
//#elif LLVM_VERSION_MAJOR >= 12
//
//    llvm::CodeGenFileType getCodeGenFileType() {
//        return llvm::CGFT_ObjectFile;
//    }
//
//#endif

    inline void codeGeneration(ModelGeneratorContext &context, std::uint32_t options) {
        EvalInitialConditionsCodeGen(context).createFunction();
        EvalReactionRatesCodeGen(context).createFunction();
        GetBoundarySpeciesAmountCodeGen(context).createFunction();
        GetFloatingSpeciesAmountCodeGen(context).createFunction();
        GetBoundarySpeciesConcentrationCodeGen(context).createFunction();
        GetFloatingSpeciesConcentrationCodeGen(context).createFunction();
        GetCompartmentVolumeCodeGen(context).createFunction();
        GetGlobalParameterCodeGen(context).createFunction();
        EvalRateRuleRatesCodeGen(context).createFunction();
        GetEventTriggerCodeGen(context).createFunction();
        GetEventPriorityCodeGen(context).createFunction();
        GetEventDelayCodeGen(context).createFunction();
        EventTriggerCodeGen(context).createFunction();
        EventAssignCodeGen(context).createFunction();
        EvalVolatileStoichCodeGen(context).createFunction();
        EvalConversionFactorCodeGen(context).createFunction();

        Function *setBoundarySpeciesAmountIR = 0;
        Function *setBoundarySpeciesConcentrationIR;
        Function *setFloatingSpeciesConcentrationIR = 0;
        Function *setCompartmentVolumeIR = 0;
        Function *setFloatingSpeciesAmountIR = 0;
        Function *setGlobalParameterIR = 0;
        Function *getFloatingSpeciesInitConcentrationsIR = 0;
        Function *setFloatingSpeciesInitConcentrationsIR = 0;
        Function *getFloatingSpeciesInitAmountsIR = 0;
        Function *setFloatingSpeciesInitAmountsIR = 0;
        Function *getBoundarySpeciesInitConcentrationsIR = 0;
        Function *setBoundarySpeciesInitConcentrationsIR = 0;
        Function *getBoundarySpeciesInitAmountsIR = 0;
        Function *setBoundarySpeciesInitAmountsIR = 0;
        Function *getCompartmentInitVolumesIR = 0;
        Function *setCompartmentInitVolumesIR = 0;
        Function *getGlobalParameterInitValueIR = 0;
        Function *setGlobalParameterInitValueIR = 0;
        if (options & LoadSBMLOptions::READ_ONLY) {
            setBoundarySpeciesAmountIR = 0;
            setBoundarySpeciesConcentrationIR = 0;
            setFloatingSpeciesConcentrationIR = 0;
            setCompartmentVolumeIR = 0;
            setFloatingSpeciesAmountIR = 0;
            setGlobalParameterIR = 0;
        } else {
            setBoundarySpeciesAmountIR =
                    SetBoundarySpeciesAmountCodeGen(context).createFunction();

            setBoundarySpeciesConcentrationIR =
                    SetBoundarySpeciesConcentrationCodeGen(context).createFunction();

            setFloatingSpeciesConcentrationIR =
                    SetFloatingSpeciesConcentrationCodeGen(context).createFunction();

            setCompartmentVolumeIR =
                    SetCompartmentVolumeCodeGen(context).createFunction();

            setFloatingSpeciesAmountIR =
                    SetFloatingSpeciesAmountCodeGen(context).createFunction();

            setGlobalParameterIR =
                    SetGlobalParameterCodeGen(context).createFunction();
        }

        if (options & LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS) {
            getFloatingSpeciesInitConcentrationsIR =
                    GetFloatingSpeciesInitConcentrationCodeGen(context).createFunction();
            setFloatingSpeciesInitConcentrationsIR =
                    SetFloatingSpeciesInitConcentrationCodeGen(context).createFunction();

            getFloatingSpeciesInitAmountsIR =
                    GetFloatingSpeciesInitAmountCodeGen(context).createFunction();
            setFloatingSpeciesInitAmountsIR =
                    SetFloatingSpeciesInitAmountCodeGen(context).createFunction();

            getBoundarySpeciesInitConcentrationsIR =
                    GetBoundarySpeciesInitConcentrationCodeGen(context).createFunction();
            setBoundarySpeciesInitConcentrationsIR =
                    SetBoundarySpeciesInitConcentrationCodeGen(context).createFunction();

            getBoundarySpeciesInitAmountsIR =
                    GetBoundarySpeciesInitAmountCodeGen(context).createFunction();
            setBoundarySpeciesInitAmountsIR =
                    SetBoundarySpeciesInitAmountCodeGen(context).createFunction();

            getCompartmentInitVolumesIR =
                    GetCompartmentInitVolumeCodeGen(context).createFunction();
            setCompartmentInitVolumesIR =
                    SetCompartmentInitVolumeCodeGen(context).createFunction();

            getGlobalParameterInitValueIR =
                    GetGlobalParameterInitValueCodeGen(context).createFunction();
            setGlobalParameterInitValueIR =
                    SetGlobalParameterInitValueCodeGen(context).createFunction();
        } else {
            getFloatingSpeciesInitConcentrationsIR = 0;
            setFloatingSpeciesInitConcentrationsIR = 0;
            getFloatingSpeciesInitAmountsIR = 0;
            setFloatingSpeciesInitAmountsIR = 0;
            getBoundarySpeciesInitConcentrationsIR = 0;
            setBoundarySpeciesInitConcentrationsIR = 0;
            getBoundarySpeciesInitAmountsIR = 0;
            setBoundarySpeciesInitAmountsIR = 0;
            setCompartmentInitVolumesIR = 0;
            getCompartmentInitVolumesIR = 0;
            getGlobalParameterInitValueIR = 0;
            setGlobalParameterInitValueIR = 0;
        }

    }

    inline std::unique_ptr<ModelGeneratorContext>
    createModelGeneratorContext(const std::string &sbml, std::uint32_t options) {

        std::unique_ptr<ModelGeneratorContext> context;

        if (options & LoadSBMLOptions::MCJIT) {
            context = std::move(
                    std::make_unique<ModelGeneratorContext>(sbml, options, std::make_unique<MCJit>(options)));
        }

#if LLVM_VERSION_MAJOR >= 13

        else if (options & LoadSBMLOptions::LLJIT) {
            context = std::move(
                    std::make_unique<ModelGeneratorContext>(sbml, options, std::make_unique<rrLLJit>(options)));
        }

#endif // LLVM_VERSION_MAJOR >= 13

        else {
            rrLogWarn << "Requested compiler not found. Defaulting to MCJit.";
            context = std::move(
                    std::make_unique<ModelGeneratorContext>(sbml, options, std::make_unique<MCJit>(options)));
        }
        return std::move(context);
    }

    ExecutableModel *
    LLVMModelGenerator::regenerateModel(rr::ExecutableModel *oldModel, libsbml::SBMLDocument *doc, uint options) {
        SharedModelResourcesPtr rc = std::make_shared<ModelResources>();

        char *docSBML = doc->toSBML();

        std::unique_ptr<ModelGeneratorContext> context = createModelGeneratorContext(
                reinterpret_cast<const char *>(docSBML), options);

        free(docSBML);

        // code generation part
        codeGeneration(*context, options);


        // optmiize the module and add it to our jit engine
        context->getJitNonOwning()->optimizeModule();
        //Save the object so we can saveState quickly later
        rc->moduleStr = context->getJitNonOwning()->getPostOptModuleStream().str().str();

        context->getJitNonOwning()->addModule();
        // now we should have jit'd code, load some function address into
        // function pointers.
        context->getJitNonOwning()->mapFunctionsToAddresses(rc, options);


        // if anything up to this point throws an exception, thats OK, because
        // we have not allocated any memory yet that is not taken care of by
        // something else.
        // Now that everything that could have thrown would have thrown, we
        // can now create the model and set its fields.

        LLVMModelData *modelData = createModelData(context->getModelDataSymbols(),
                                                   context->getRandom());

        uint llvmsize = ModelDataIRBuilder::getModelDataSize(
                context->getJitNonOwning()->getModuleNonOwning(),
                context->getJitNonOwning()->getDataLayout()
        );

        if (llvmsize != modelData->size) {
            std::stringstream s;

            s << "LLVM Model Data size " << llvmsize << " is different from " <<
              "C++ size of LLVM ModelData, " << modelData->size;

            LLVMModelData_free(modelData);

            rrLog(Logger::LOG_FATAL) << s.str();

            throw_llvm_exception(s.str());
        }

        // * MOVE * the bits over from the context to the exe model.
        context->transferObjectsToResources(rc);
//        rc->moduleStr = moduleStr;

        LLVMExecutableModel *newModel = new LLVMExecutableModel(rc, modelData);


        if (oldModel) {
            // the stored SBML document will not keep updated, so we need to
            // copy the old values (e.g, initial values, current values) to new model
            // so that we can start from where we paused

            std::vector<std::string> newSymbols = newModel->getRateRuleSymbols();

            for (int i = 0; i < oldModel->getNumFloatingSpecies(); i++) {
                std::string id = oldModel->getFloatingSpeciesId(i);
                int index = newModel->getFloatingSpeciesIndex(id);

                if (index >= 0 && index < newModel->modelData->numInitFloatingSpecies) {
                    // new model has this species

                    if (!newModel->symbols->hasAssignmentRule(id) && !newModel->symbols->hasRateRule(id)) {
                        if (!newModel->symbols->hasInitialAssignmentRule(id)) {
                            double initValue = 0;
                            oldModel->getFloatingSpeciesInitAmounts(1, &i, &initValue);
                            newModel->modelData->initFloatingSpeciesAmountsAlias[index] = initValue;
                            //newModel->setFloatingSpeciesInitAmounts(1, &index, &initValue);
                        }
                    }
                }
            }


            for (int i = 0; i < oldModel->getNumFloatingSpecies(); i++) {
                std::string id = oldModel->getFloatingSpeciesId(i);
                int index = newModel->getFloatingSpeciesIndex(id);

                if (index >= 0 && index < newModel->modelData->numIndFloatingSpecies) {
                    // new model has this species

                    // TODO: should we change the dirty flag when we call setters?
                    double value = 0;
                    oldModel->getFloatingSpeciesAmounts(1, &i, &value);

                    if (!newModel->symbols->hasAssignmentRule(id) && !newModel->symbols->hasRateRule(id)) {
                        newModel->modelData->floatingSpeciesAmountsAlias[index] = value;
                        //newModel->setFloatingSpeciesAmounts(1, &index, &value);
                    } else if (newModel->symbols->hasRateRule(id)) {
                        // copy to rate rule value data block
                        std::vector<std::string>::iterator it = std::find(newSymbols.begin(), newSymbols.end(), id);
                        if (it != newSymbols.end()) {
                            // found it
                            index = std::distance(newSymbols.begin(), it);
                            newModel->modelData->rateRuleValuesAlias[index] = value;
                        }

                    }
                }
            }


            for (int i = 0; i < oldModel->getNumBoundarySpecies(); i++) {
                std::string id = oldModel->getBoundarySpeciesId(i);
                int index = newModel->getBoundarySpeciesIndex(id);

                // TODO: set concentration or amount?

                if (index >= 0 && index < newModel->modelData->numIndBoundarySpecies) {
                    // new model has this species

                    double value = 0;
                    oldModel->getBoundarySpeciesAmounts(1, &i, &value);

                    if (!newModel->symbols->hasAssignmentRule(id) && !newModel->symbols->hasRateRule(id)) {
                        newModel->modelData->boundarySpeciesAmountsAlias[index] = value;
                        //newModel->setBoundarySpeciesAmounts(1, &index, &value);
                    } else if (newModel->symbols->hasRateRule(id)) {
                        // copy to rate rule value data block
                        std::vector<std::string>::iterator it = std::find(newSymbols.begin(), newSymbols.end(), id);
                        if (it != newSymbols.end()) {
                            // found it
                            index = std::distance(newSymbols.begin(), it);
                            newModel->modelData->rateRuleValuesAlias[index] = value;
                        }
                    } else {
                        if (!newModel->symbols->hasInitialAssignmentRule(id)) {
                            double initValue = 0;
                            oldModel->getBoundarySpeciesInitAmounts(1, &i, &initValue);
                            newModel->modelData->initBoundarySpeciesAmountsAlias[index] = initValue;
                        }
                    }
                }

            }


            for (int i = 0; i < oldModel->getNumCompartments(); i++) {
                std::string id = oldModel->getCompartmentId(i);
                int index = newModel->getCompartmentIndex(id);

                if (index >= 0 && index < newModel->modelData->numInitCompartments) {
                    // new model has this compartment

                    if (!newModel->symbols->hasAssignmentRule(id) && !newModel->symbols->hasRateRule(id)) {
                        if (!newModel->symbols->hasInitialAssignmentRule(id)) {
                            double initValue = 0;
                            oldModel->getCompartmentInitVolumes(1, &i, &initValue);
                            newModel->modelData->initCompartmentVolumesAlias[index] = initValue;
                            //newModel->setCompartmentInitVolumes(1, &index, &initValue);
                        }

                    }

                }

            }


            for (int i = 0; i < oldModel->getNumCompartments(); i++) {
                std::string id = oldModel->getCompartmentId(i);
                int index = newModel->getCompartmentIndex(id);

                if (index >= 0 && index < newModel->modelData->numIndCompartments) {
                    // new model has this compartment
                    double value = 0;
                    oldModel->getCompartmentVolumes(1, &i, &value);

                    if (!newModel->symbols->hasAssignmentRule(id) && !newModel->symbols->hasRateRule(id)) {
                        newModel->modelData->compartmentVolumesAlias[index] = value;
                        //newModel->setCompartmentVolumes(1, &index, &value);
                    } else if (newModel->symbols->hasRateRule(id)) {
                        // copy to rate rule value data block
                        std::vector<std::string>::iterator it = std::find(newSymbols.begin(), newSymbols.end(), id);
                        if (it != newSymbols.end()) {
                            // found it
                            index = std::distance(newSymbols.begin(), it);
                            newModel->modelData->rateRuleValuesAlias[index] = value;
                        }

                    }

                }

            }

            for (int i = 0; i < oldModel->getNumGlobalParameters(); i++) {
                std::string id = oldModel->getGlobalParameterId(i);
                int index = newModel->getGlobalParameterIndex(id);

                if (index >= 0 && index < newModel->modelData->numInitGlobalParameters) {
                    // new model has this parameter

                    if (!newModel->symbols->hasAssignmentRule(id) && !newModel->symbols->hasRateRule(id)) {
                        if (!newModel->symbols->hasInitialAssignmentRule(id)) {
                            double initValue = 0;
                            oldModel->getGlobalParameterInitValues(1, &i, &initValue);
                            newModel->modelData->initGlobalParametersAlias[index] = initValue;
                            //newModel->setGlobalParameterInitValues(1, &index, &initValue);
                        }
                    }
                }
            }


            for (int i = 0; i < oldModel->getNumGlobalParameters(); i++) {
                std::string id = oldModel->getGlobalParameterId(i);
                int index = newModel->getGlobalParameterIndex(id);

                if (index >= 0 && index < newModel->modelData->numIndGlobalParameters) {
                    // new model has this parameter

                    double value = 0;
                    oldModel->getGlobalParameterValues(1, &i, &value);

                    if (!newModel->symbols->hasAssignmentRule(id) && !newModel->symbols->hasRateRule(id)) {
                        newModel->modelData->globalParametersAlias[index] = value;
                        //newModel->setGlobalParameterValues(1, &index, &value);
                    } else if (newModel->symbols->hasRateRule(id)) {
                        // copy to rate rule value data block
                        std::vector<std::string>::iterator it = std::find(newSymbols.begin(), newSymbols.end(), id);
                        if (it != newSymbols.end()) {
                            // found it
                            index = std::distance(newSymbols.begin(), it);
                            newModel->modelData->rateRuleValuesAlias[index] = value;
                        }
                    }
                }
            }
            newModel->setTime(oldModel->getTime());
        }

        return newModel;
    }


    ExecutableModel *LLVMModelGenerator::createModel(const std::string &sbml, std::uint32_t options) {
        bool forceReCompile = options & LoadSBMLOptions::RECOMPILE;

        std::string md5;

        // if we force recompile, then we don't need to think
        // about locating a previously compiled model
        if (!forceReCompile) {
            // check for a cached copy
            md5 = rr::getMD5(sbml);

            if (options & LoadSBMLOptions::CONSERVED_MOIETIES) {
                md5 += "_conserved";
            }

            SharedModelResourcesPtr sp;

            cachedModelsMutex.lock();

            // if count == 1, key is in map. Keys are unique so can only be 1 or 0.
            if (cachedModelResources.count(md5) == 1) {
                // if key found, lock the weak pointer into shared pointer
                sp = cachedModelResources.at(md5).lock();
            }

            cachedModelsMutex.unlock();

            // we could have recieved a bad ptr, a model could have been deleted,
            // in which case, we should have a bad ptr.

            if (sp) {
                rrLog(Logger::LOG_DEBUG) << "found a cached model for \"" << md5 << "\"";
                return new LLVMExecutableModel(sp, createModelData(*sp->symbols, sp->random));
            } else {
                rrLog(Logger::LOG_DEBUG) << "no cached model found for " << md5
                                         << ", creating new one";
            }
        }

        SharedModelResourcesPtr rc = std::make_shared<ModelResources>();

        std::unique_ptr<ModelGeneratorContext> context = createModelGeneratorContext(sbml, options);

        // Do all code generation here. This populates the IR module representing
        // this sbml model.
//        codeGeneration(*context, options);
        EvalInitialConditionsCodeGen(*context).createFunction();


        auto Ctx_ = std::make_unique<llvm::LLVMContext>();
//        llvm::Module j("di", *Ctx)
        auto M_ = std::make_unique<llvm::Module>("Fibaroo", *Ctx_);

        // Create the fib function and insert it into the model
        // This function is said to return an int and take an int
        // as parameter
        llvm::FunctionType *FibFTy = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(*Ctx_), {llvm::Type::getInt32Ty(*Ctx_)}, false
        );

        llvm::Function *FibFn = llvm::Function::Create(FibFTy, llvm::Function::ExternalLinkage, "fibd", *M_);

        //Add a basic memory block
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(*Ctx_, "EntryBlock", FibFn);

        // get pointers to the constants
        llvm::Value *One = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Ctx_), 1);
        llvm::Value *Two = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Ctx_), 2);

        // get ptr tointeger arg of the function
        llvm::Argument *ArgX = &*FibFn->arg_begin();
        ArgX->setName("AnArg");

        // the true block
        llvm::BasicBlock *RetBB = llvm::BasicBlock::Create(*Ctx_, "return", FibFn);

        // and the exit block
        llvm::BasicBlock *RecurseB = llvm::BasicBlock::Create(*Ctx_, "recurse", FibFn);

        // create the if arg < 2 goto exit
        llvm::Value *CondInst = new llvm::ICmpInst(*BB, llvm::ICmpInst::ICMP_SLE, ArgX, Two, "cond");
        llvm::BranchInst::Create(RetBB, RecurseB, CondInst, BB);
        llvm::ReturnInst::Create(*Ctx_, One, RetBB);

        llvm::Value *sub = llvm::BinaryOperator::CreateSub(ArgX, One, "arg", RecurseB);
        llvm::Value *CallFibX1 = llvm::CallInst::Create(FibFn, sub, "fibx1", RecurseB);

        // create fib(x-2)
        sub = llvm::BinaryOperator::CreateSub(ArgX, Two, "arg", RecurseB);
        llvm::Value *CallFibX2 = llvm::CallInst::Create(FibFn, sub, "fibx2", RecurseB);

        // fib(x-1)+fib(x-2)
        llvm::Value *Sum = llvm::BinaryOperator::CreateAdd(CallFibX1, CallFibX2, "addresult", RecurseB);

        // Create the return instruction and add it to the basic block
        llvm::ReturnInst::Create(*Ctx_, Sum, RecurseB);







        // optimize the module and add it to our jit engine

        context->getJitNonOwning()->optimizeModule();

        // Save the string representation so we can saveState quickly later
        rc->moduleStr = context->getJitNonOwning()->getPostOptModuleStream().str().str();

        // actually add the module, which is a member variable
        // of the Jit to the jit engine.
        //context->getJitNonOwning()->addModule();

        context->getJitNonOwning()->addModule(std::move(M_), std::move(Ctx_));

        using fibonacciFnPtr1 = int (*)(int);
        fibonacciFnPtr1 fibPtr = (int (*)(int)) context->getJitNonOwning()->lookupFunctionAddress("fibd");

        std::cout << "You are here: " << std::endl;
        std::cout << fibPtr(20) << std::endl;
        /**
         * Up until this point we've been creating IR code
         * and creating the module. Now we need to grab
         * pointers to the various bits of compiled code.
         */
        context->getJitNonOwning()->addModule(
                std::move(context->getJitNonOwning()->module),
                std::move(context->getJitNonOwning()->context)
        );
        // load some function address into function pointers.
        context->getJitNonOwning()->mapFunctionsToAddresses(rc, options);




        // if anything up to this point throws an exception, thats OK, because
        // we have not allocated any memory yet that is not taken care of by
        // something else.
        // Now that everything that could have thrown would have thrown, we
        // can now create the model and set its fields.

        LLVMModelData *modelData = createModelData(context->getModelDataSymbols(),
                                                   context->getRandom());

        uint llvmsize = ModelDataIRBuilder::getModelDataSize(context->getJitNonOwning()->getModuleNonOwning(),
                                                             context->getJitNonOwning()->getDataLayout());

        if (llvmsize != modelData->size) {
            std::stringstream s;

            s << "LLVM Model Data size " << llvmsize << " is different from " <<
              "C++ size of LLVM ModelData, " << modelData->size;

            LLVMModelData_free(modelData);

            rrLog(Logger::LOG_FATAL) << s.str();

            throw_llvm_exception(s.str());
        }

        // * MOVE * the bits over from the context to the exe model.
        context->transferObjectsToResources(rc);

        if (!forceReCompile) {
            // check for a chached copy, another thread could have
            // created one while we were making ours...

            ModelResourcesPtrMap::const_iterator i;

            SharedModelResourcesPtr sp;

            cachedModelsMutex.lock();

            // whilst we have it locked, clear any expired ptrs
            for (ModelResourcesPtrMap::const_iterator j = cachedModelResources.begin();
                 j != cachedModelResources.end();) {
                if (j->second.expired()) {
                    rrLog(Logger::LOG_DEBUG) <<
                                             "removing expired model resource for hash " << md5;

                    j = cachedModelResources.erase(j);
                } else {
                    ++j;
                }
            }

            if ((i = cachedModelResources.find(md5)) == cachedModelResources.end()) {
                rrLog(Logger::LOG_DEBUG) << "could not find existing cached resource "
                                            "resources, for hash " << md5 <<
                                         ", inserting new resources into cache";

                cachedModelResources[md5] = rc;
            }

            cachedModelsMutex.unlock();
        }

        return new LLVMExecutableModel(rc, modelData);
    }



/************ LLVM Utility Functions, TODO: Move To Separate File ************/

/**
 * C++ 11 style to_string for LLVM types
 */
    std::string to_string(const llvm::Value *value) {
        std::string str;
        llvm::raw_string_ostream stream(str);
        value->print(stream);
        return str;
    }

    LLVMModelData *createModelData(const rrllvm::LLVMModelDataSymbols &symbols, const Random *random) {
        uint modelDataBaseSize = sizeof(LLVMModelData);

        uint numIndCompartments = static_cast<uint>(symbols.getIndependentCompartmentSize());
        uint numIndFloatingSpecies = static_cast<uint>(symbols.getIndependentFloatingSpeciesSize());
        uint numIndBoundarySpecies = static_cast<uint>(symbols.getIndependentBoundarySpeciesSize());
        uint numIndGlobalParameters = static_cast<uint>(symbols.getIndependentGlobalParameterSize());

        uint numInitCompartments = static_cast<uint>(symbols.getInitCompartmentSize());
        uint numInitFloatingSpecies = static_cast<uint>(symbols.getInitFloatingSpeciesSize());
        uint numInitBoundarySpecies = static_cast<uint>(symbols.getInitBoundarySpeciesSize());
        uint numInitGlobalParameters = static_cast<uint>(symbols.getInitGlobalParameterSize());

        // no initial conditions for these
        uint numRateRules = static_cast<uint>(symbols.getRateRuleSize());
        uint numReactions = static_cast<uint>(symbols.getReactionSize());

        uint modelDataSize = modelDataBaseSize +
                             sizeof(double) * (
                                     numIndCompartments +
                                     numInitCompartments +
                                     numInitFloatingSpecies +
                                     numIndBoundarySpecies +
                                     numInitBoundarySpecies +
                                     numIndGlobalParameters +
                                     numInitGlobalParameters +
                                     numReactions +
                                     numRateRules +
                                     numIndFloatingSpecies
                             );

        LLVMModelData *modelData = (LLVMModelData *) calloc(
                modelDataSize, sizeof(unsigned char));

        modelData->size = modelDataSize;
        modelData->numIndCompartments = numIndCompartments;
        modelData->numIndFloatingSpecies = numIndFloatingSpecies;
        modelData->numIndBoundarySpecies = numIndBoundarySpecies;
        modelData->numIndGlobalParameters = numIndGlobalParameters;

        modelData->numInitCompartments = numInitCompartments;
        modelData->numInitFloatingSpecies = numInitFloatingSpecies;
        modelData->numInitBoundarySpecies = numInitBoundarySpecies;
        modelData->numInitGlobalParameters = numInitGlobalParameters;

        modelData->numRateRules = numRateRules;
        modelData->numReactions = numReactions;
        modelData->numEvents = static_cast<uint>(symbols.getEventAttributes().size());

        // set the aliases to the offsets
        uint offset = 0;

        modelData->compartmentVolumesAlias = &modelData->data[offset];
        offset += numIndCompartments;

        modelData->initCompartmentVolumesAlias = &modelData->data[offset];
        offset += numInitCompartments;

        modelData->initFloatingSpeciesAmountsAlias = &modelData->data[offset];
        offset += numInitFloatingSpecies;

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

        assert (modelDataBaseSize + offset * sizeof(double) == modelDataSize &&
                "LLVMModelData size not equal to base size + data");

        // allocate the stoichiometry matrix
        const std::vector<uint> &stoichRowIndx = symbols.getStoichRowIndx();
        const std::vector<uint> &stoichColIndx = symbols.getStoichColIndx();
        std::vector<double> stoichValues(stoichRowIndx.size(), 0);

        modelData->stoichiometry = rr::csr_matrix_new(numIndFloatingSpecies, numReactions,
                                                      stoichRowIndx, stoichColIndx, stoichValues);

        // make a copy of the random object
        modelData->random = random ? new Random(*random) : 0;

        return modelData;
    }

} /* namespace rrllvm */


