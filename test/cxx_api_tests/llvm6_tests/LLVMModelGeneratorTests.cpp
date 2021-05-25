//
// Created by Ciaran on 25/05/2021.
//

#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "llvm/LLVMModelGenerator.h"
#include "llvm/LLVMExecutableModel.h"
#include "rrRoadRunnerOptions.h" // for LoadSBMLOptions

using namespace rrllvm;
using namespace rr;

class LLVMModelGeneratorTests : public ::testing::Test {
public :

    LLVMModelGeneratorTests() = default;
};

TEST_F(LLVMModelGeneratorTests, createModel){
    SimpleFlux simpleFlux;
    unsigned int x = LoadSBMLOptions::RECOMPILE;
    std::cout << x << std::endl;
//    LoadSBMLOptions::setItem(LoadSBMLOptions::RECOMPILE,  0);
//    LoadSBMLOptions options;
//
//    options.modelGeneratorOpt << std::endl;


    /**
     * Right, I haven't fully worked out what the "options" argument actually does.
     * Its a single number but not associated with any switch statements, so why is
     * it called "options"??? It looks as though the unsigned options argument
     * is just been used an a bool. So why not use a bool?
     */
    ExecutableModel* executableModel = LLVMModelGenerator::createModel(simpleFlux.str(), 0);


}



























