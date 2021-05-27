//
// Created by Ciaran on 25/05/2021.
//

#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "llvm/LLVMModelGenerator.h"
#include "llvm/LLVMExecutableModel.h"
#include "rrRoadRunnerOptions.h" // for LoadSBMLOptions
#include "rrConfig.h"

using namespace rrllvm;
using namespace rr;

/**
 * @brief test model creation with LLVMModelGenerator
 * @note LLVMModelGenerator only contains two static functions
 * which are "friend" functions to the LLVMExecutableModel anyway.
 * My opinion (cw) is that these two functions should be merged
 * with LLVMExecutbaleModel.
 * @details We test here the ability to create Models under different
 * settings and sitations. Testing specifics of model creation
 * however is tested in other units.
 */
class LLVMModelGeneratorTests : public ::testing::Test {
public :

    LLVMModelGeneratorTests() = default;
};

TEST_F(LLVMModelGeneratorTests, createModelWithForceRecompileTrue){
    SimpleFlux simpleFlux;
    std:uint32_t options = 0;
    // turn the bit that encodes option for recompiling sbml to on
    options |= LoadSBMLOptions::RECOMPILE;
    ASSERT_TRUE((bool)options); // make sure we've turned on Recompile flag
    // how can we test that RECOMPILE flag actually had any effect?
    // for now, we just make sure we can create the model without throwing error
    ASSERT_NO_THROW(
        ExecutableModel* executableModel = LLVMModelGenerator::createModel(simpleFlux.str(), options);
        LLVMExecutableModel m = *dynamic_cast<LLVMExecutableModel*>(executableModel);
    );
}

/**
 * When we create a model and we do not want to force recompile and we do not already
 * have a model cached
 */
TEST_F(LLVMModelGeneratorTests, createModelWithForceRecompileSetToFalseAndModelNotAlreadyExists){
    SimpleFlux simpleFlux;
    std:uint32_t options = 0;
    // turn the bit that encodes option for recompiling sbml to on
    options &= ~LoadSBMLOptions::RECOMPILE;
    ASSERT_FALSE((bool)options); // make sure we've turned on Recompile flag
    ASSERT_NO_THROW(
        ExecutableModel* executableModel = LLVMModelGenerator::createModel(simpleFlux.str(), options);
        LLVMExecutableModel m = *dynamic_cast<LLVMExecutableModel*>(executableModel);
    );
}

/**
 * When we create a model and we do not want to force recompile and we have a
 * cached model ready so that we do not need to recompile
 */
TEST_F(LLVMModelGeneratorTests, createModelWithForceRecompileSetToFalseAndModelAlreadyExists){
    SimpleFlux simpleFlux;
    std:uint32_t options = 0;
    // turn the bit that encodes option for recompiling sbml to on
    options |= LoadSBMLOptions::RECOMPILE;
    ASSERT_TRUE((bool)options); // make sure we've turned on Recompile flag
    ExecutableModel* executableModel = LLVMModelGenerator::createModel(simpleFlux.str(), options);

}



























