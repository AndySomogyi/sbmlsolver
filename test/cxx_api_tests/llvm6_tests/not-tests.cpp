#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "sbml/SBMLTypes.h"

#include "TestModelFactory.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include "rrRoadRunner.h"

using namespace libsbml;
using namespace llvm;

/**
 * These are not actually tests. Just space for me to begin figuring out the llvm stuff
 */

class NotTests : public ::testing::Test {
public:
    NotTests() = default;
};

TEST_F(NotTests, i){
    rr::RoadRunner rr(SimpleFlux().str());
}

TEST_F(NotTests, t){
    LLVMContext TheContext;
    IRBuilder<> Builder(TheContext);
    std::unique_ptr<Module> TheModule;
    std::map<std::string, Value *> NamedValues;

    ConstantFP* constantFp = ConstantFP::get(TheContext, APFloat(4.4));


    SimpleFlux simpleFlux;
    SBMLReader reader;
    SBMLDocument* doc = reader.readSBMLFromString(simpleFlux.str());

}

TEST_F(NotTests, lg){
    bool truth = false;
    std::cout << truth << std::endl;
    truth |= true;
    std::cout << truth << std::endl;
}



































