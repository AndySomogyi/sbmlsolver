//
// Created by Ciaran on 25/10/2021.
//

#include "llvm/MCJit.h"
#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrLogger.h"
#include "rrRoadRunnerOptions.h"
#include "JitTests.h"

using namespace rr;
using namespace rrllvm;

class LLJitTests : public JitTests {
public:
    LLJitTests() = default;

};

using fibonacciFnPtr = int (*)(int);

TEST_F(LLJitTests, ModuleNonOwningNotNull) {
    LoadSBMLOptions opt;
//    LLJit llJit(opt.modelGeneratorOpt);
//    ASSERT_TRUE(mcJit.getModuleNonOwning());
}

//TEST_F(LLJitTests, ContextNonOwningNotNull) {
//    LoadSBMLOptions opt;
//    MCJit mcJit(opt.modelGeneratorOpt);
//    ASSERT_TRUE(mcJit.getContextNonOwning());
//}
//
//TEST_F(LLJitTests, BuilderNonOwningNotNull) {
//    LoadSBMLOptions opt;
//    MCJit mcJit(opt.modelGeneratorOpt);
//    ASSERT_TRUE(mcJit.getBuilderNonOwning());
//}
//
//TEST_F(LLJitTests, CreateJittedFibonacci) {
//    // maybe the module and shouldnt be owned by the jit?:?
//    LoadSBMLOptions opt;
//    MCJit mcJit(opt.modelGeneratorOpt);
//    CreateFibFunction(mcJit.getModuleNonOwning());
////    std::cout << mcJit.emitToString();
//    fibonacciFnPtr fibPtr = (int (*)(int)) mcJit.getFunctionAddress("fib");
//    ASSERT_EQ(fibPtr(4), 3);
//}
//
//TEST_F(LLJitTests, TransferObjectsToResources) {
//    ASSERT_FALSE(true);
//}
//
//TEST_F(LLJitTests, t) {
//    RoadRunner rr(OpenLinearFlux().str());
//    auto data = rr.simulate(0 , 10, 11);
//    std::cout << *data << std::endl;
//}
//
//
//
//
//
//
//
//
//
//

