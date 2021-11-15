//
// Created by Ciaran on 15/11/2021.
//

#ifndef ROADRUNNER_LLVMGENERATEDFUNCTIONTESTS_H
#define ROADRUNNER_LLVMGENERATEDFUNCTIONTESTS_H

#include "gtest/gtest.h"
#include "rrLogger.h"
#include "LLVMModelData.h"
#include "sbml/SBMLTypes.h"
#include "rrRoadRunnerOptions.h"
#include "JitFactory.h"

#include "EvalInitialConditionsCodeGen.h"
#include "GetValuesCodeGen.h"

using namespace rr;
using namespace rrllvm;
using namespace libsbml;

/**
 * These tests, and those which derive from it
 * are not yet a complete test suite. Instead they
 * serve to help me debug some issues and might as
 * well persist. Eventually it'd be nice to
 * expand on these tests (and see Derived classes)
 * to get comprehensive coverage of the llvm generated
 * code.
 */


/**
 * @brief Test suite for llvm part of the code
 */
class LLVMGeneratedFunctionTests : public ::testing::Test {
public:
    LLVMGeneratedFunctionTests() {
        Logger::setLevel(Logger::LOG_INFORMATION);
    }

    void checkInitialEvaluationSpecies(int options, int speciesAmt, double compVol, double expectedConc) {

        // create a sbml model. It has a single species in a Cell compartment.
        std::unique_ptr<SBMLDocument> doc = std::make_unique<SBMLDocument>(3, 2);
        Model *m = doc->createModel("Check Eval Init Cond");

        Compartment *c = m->createCompartment();
        c->setId("cell");
        c->setVolume(compVol);
        c->setSpatialDimensions(3.0);

        Species *s = m->createSpecies();
        s->setCompartment("cell");
        s->setName("S1");
        s->setInitialAmount(speciesAmt); // so 4.5 after initial eval?

        // create the Jit engine and give to ModelGeneratorContext.
        std::unique_ptr<Jit> j = JitFactory::makeJitEngine(options);
        ModelGeneratorContext mgc(doc.get() /*borrowed*/, options, std::move(j));

        // code gen (limited scope, this test only needs a few functions)
        EvalInitialConditionsCodeGen(mgc).createFunction();
        GetFloatingSpeciesConcentrationCodeGen(mgc).createFunction();
        GetFloatingSpeciesAmountCodeGen(mgc).createFunction();

        // Do the compilation
        mgc.getJitNonOwning()->addModule();

        // load the necessary function pointers
        EvalInitialConditionsCodeGen::FunctionPtr evalInitConditions
                = reinterpret_cast<EvalInitialConditionsCodeGen::FunctionPtr>(
                        mgc.getJitNonOwning()->lookupFunctionAddress(EvalInitialConditionsCodeGen::FunctionName)
                );
        GetFloatingSpeciesConcentrationCodeGen::FunctionPtr getFloatingSpeciesConc
                = reinterpret_cast<GetFloatingSpeciesConcentrationCodeGen::FunctionPtr>(
                        mgc.getJitNonOwning()->lookupFunctionAddress(
                                GetFloatingSpeciesConcentrationCodeGen::FunctionName)
                );
        GetFloatingSpeciesAmountCodeGen::FunctionPtr getFloatingSpeciesAmt
                = reinterpret_cast<GetFloatingSpeciesAmountCodeGen::FunctionPtr>(
                        mgc.getJitNonOwning()->lookupFunctionAddress(GetFloatingSpeciesAmountCodeGen::FunctionName)
                );

        // we need an instance of modelData to work with the functions.
        LLVMModelDataSymbols sym(m, options);
        std::unique_ptr<Random> r = std::make_unique<Random>();
        LLVMModelData *modelData = createModelData(sym, r.get());

        // Evaluate initial conditions
        evalInitConditions(modelData, options);

        // Check Species zero's value against expected quantities.
        // Our model only has a single species, therefore it is at
        // idx 0.
        double S1Amt = getFloatingSpeciesAmt(modelData, 0);
        double S1Conc = getFloatingSpeciesConc(modelData, 0);
        std::cout << S1Amt << std::endl;
        std::cout << S1Conc << std::endl;
        ASSERT_EQ(expectedConc, S1Amt);

    }
};


#endif //ROADRUNNER_LLVMGENERATEDFUNCTIONTESTS_H
