//
// Created by Ciaran on 25/10/2021.
//

#include "llvm/LLJit.h"
#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrLogger.h"
#include "rrRoadRunnerOptions.h"
#include "JitTests.h"
#include "rrConfig.h"

using namespace rr;
using namespace rrllvm;

class LLJitTests : public JitTests {
public:
    LLJitTests() {
        Logger::setLevel(Logger::LOG_DEBUG);
    };

};

using fibonacciFnPtr = int (*)(int);

TEST_F(LLJitTests, ModuleNonOwningNotNull) {
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    ASSERT_TRUE(llJit.getModuleNonOwning());
}

TEST_F(LLJitTests, ContextNonOwningNotNull) {
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    ASSERT_TRUE(llJit.getContextNonOwning());
}

TEST_F(LLJitTests, BuilderNonOwningNotNull) {
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    ASSERT_TRUE(llJit.getBuilderNonOwning());
}

TEST_F(LLJitTests, CreateJittedFibonacci) {
    // maybe the module and shouldnt be owned by the jit?:?
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    CreateFibFunction(llJit.getModuleNonOwning());
    std::cout << llJit.emitToString();
    llJit.addIRModule();
    fibonacciFnPtr fibPtr = (int (*)(int)) llJit.lookupFunctionAddress("fib");
    ASSERT_EQ(fibPtr(4), 3);
}

TEST_F(LLJitTests, FibonacciCached) {
    // maybe the module and shouldnt be owned by the jit?:?
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    llJit.setModuleIdentifier("fibMod");
    CreateFibFunction(llJit.getModuleNonOwning());
//    std::cout << llJit.emitToString();
    llJit.addIRModule();
    fibonacciFnPtr fibPtr1 = (int (*)(int)) llJit.lookupFunctionAddress("fib");
    ASSERT_EQ(fibPtr1(4), 3);


    LLJit llJit2(opt.modelGeneratorOpt);
    llJit2.setModuleIdentifier("fibMod");
    CreateFibFunction(llJit2.getModuleNonOwning());
//    std::cout << llJit2.emitToString();
    llJit2.addIRModule();
    fibonacciFnPtr fibPtr2 = (int (*)(int)) llJit2.lookupFunctionAddress("fib");
    ASSERT_EQ(fibPtr2(4), 3);


    llvm::LLVMContext ctx;
    auto mod = std::make_unique<llvm::Module>("fibMod", ctx);
    mod->setModuleIdentifier("fibMod");

    std::unique_ptr<llvm::MemoryBuffer> obj = SBMLModelObjectCache::getObjectCache().getObject(mod.get());


//    std::cout << obj << std::endl;

}

TEST_F(LLJitTests, LoadPowerFunctiond) {
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    llvm::Function* fn = llJit.getModuleNonOwning()->getFunction("pow");
    llJit.addModule();
    ASSERT_TRUE(fn);
}

TEST_F(LLJitTests, SetModuleIdentifier) {
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    llJit.setModuleIdentifier("FibMod");
    ASSERT_STREQ("FibMod", llJit.getModuleNonOwning()->getModuleIdentifier().c_str());
}

TEST_F(LLJitTests, CreateUsingSBMLString) {
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);

    llJit.setModuleIdentifier("FibMod");
    ASSERT_STREQ("FibMod", llJit.getModuleNonOwning()->getModuleIdentifier().c_str());
}

TEST_F(LLJitTests, ToObjectFile) {
    /**
     * default lljit uses RTDyldObjectLinkingLayer
     */
    LoadSBMLOptions opt;
    LLJit llJit(opt.modelGeneratorOpt);
    CreateFibFunction(llJit.getModuleNonOwning());
    llJit.addModule();

//    llJit.getLLJitNonOwning()->getExecutionSession().dispatchTask()
//
//    std::string s;
//    llvm::raw_string_ostream os(s);
//    llJit.getLLJitNonOwning()->getExecutionSession().dump(os);
//    std::cout << s << std::endl;


//    std::unique_ptr<llvm::orc::MaterializationResponsibility> mr =
//            std::make_unique<llvm::orc::MaterializationResponsibility>();
//
//    std::unique_ptr<llvm::MemoryBuffer> buf = std::make_unique<llvm::MemoryBuffer>();
//
////    llJit.getLLJitNonOwning()->getObjLinkingLayer().emit(std::move(mr), std::move(buf));
//    llJit.getLLJitNonOwning()->getIRTransformLayer().emit()
}


/**
 * Not concerned with simulation accuracy here, only that the sbml
 * compiles and the model simulates.
 */
TEST_F(LLJitTests, CheckModelSimulates) {
//    rr::Config::setValue(rr::Config::LLVM_COMPILER, rr::Config::LLVM_COMPILER_VALUES::MCJIT);
    rr::Config::setValue(rr::Config::LLVM_COMPILER, rr::Config::LLVM_COMPILER_VALUES::LLJIT);
    RoadRunner rr(OpenLinearFlux().str());
    auto data = rr.simulate(0, 10, 11);
    ASSERT_EQ(typeid(*data), typeid(ls::Matrix<double>));
}
