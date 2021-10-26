//
// Created by Ciaran on 25/10/2021.
//

#include "llvm/MCJit.h"
#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrLogger.h"
#include "rrRoadRunnerOptions.h"

using namespace rr;
using namespace rrllvm;

class MCJitTests : public ::testing::Test {
public:
    MCJitTests() = default;

//    template<typename FnPtr>
//    void checkLibFn(const std::string& funcName, double expected) {
//        using namespace llvm;
//        LoadSBMLOptions opt;
//        MCJit mcJit(opt.modelGeneratorOpt);
//        FnPtr fnPtr = (FnPtr)mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
//        double actual = fnPtr(2, 4);
//    }


    static llvm::Function *CreateFibFunction(llvm::Module *M) {
        llvm::LLVMContext &ctx = M->getContext();

        // Create the fib function and insert it into the model
        // This function is said to return an int and take an int
        // as parameter
        llvm::FunctionType *FibFTy = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(ctx), {llvm::Type::getInt32Ty(ctx)}, false
        );

        llvm::Function *FibFn = llvm::Function::Create(FibFTy, llvm::Function::ExternalLinkage, "fib", M);

        //Add a basic memory block
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "EntryBlock", FibFn);

        // get pointers to the constants
        llvm::Value *One = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 1);
        llvm::Value *Two = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 2);

        // get ptr tointeger arg of the function
        llvm::Argument *ArgX = &*FibFn->arg_begin();
        ArgX->setName("AnArg");

        // the true block
        llvm::BasicBlock *RetBB = llvm::BasicBlock::Create(ctx, "return", FibFn);

        // and the exit block
        llvm::BasicBlock *RecurseB = llvm::BasicBlock::Create(ctx, "recurse", FibFn);

        // create the if arg < 2 goto exit
        llvm::Value *CondInst = new llvm::ICmpInst(*BB, llvm::ICmpInst::ICMP_SLE, ArgX, Two, "cond");
        llvm::BranchInst::Create(RetBB, RecurseB, CondInst, BB);
        llvm::ReturnInst::Create(ctx, One, RetBB);

        llvm::Value *sub = llvm::BinaryOperator::CreateSub(ArgX, One, "arg", RecurseB);
        llvm::Value *CallFibX1 = llvm::CallInst::Create(FibFn, sub, "fibx1", RecurseB);

        // create fib(x-2)
        sub = llvm::BinaryOperator::CreateSub(ArgX, Two, "arg", RecurseB);
        llvm::Value *CallFibX2 = llvm::CallInst::Create(FibFn, sub, "fibx2", RecurseB);

        // fib(x-1)+fib(x-2)
        llvm::Value *Sum = llvm::BinaryOperator::CreateAdd(CallFibX1, CallFibX2, "addresult", RecurseB);

        // Create the return instruction and add it to the basic block
        llvm::ReturnInst::Create(ctx, Sum, RecurseB);

        return FibFn;
    }
};

using fibonacciFnPtr = int (*)(int);

TEST_F(MCJitTests, ModuleNonOwningNotNull) {
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    ASSERT_TRUE(mcJit.getModuleNonOwning());
}

TEST_F(MCJitTests, ContextNonOwningNotNull) {
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    ASSERT_TRUE(mcJit.getContextNonOwning());
}

TEST_F(MCJitTests, BuilderNonOwningNotNull) {
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    ASSERT_TRUE(mcJit.getBuilderNonOwning());
}

TEST_F(MCJitTests, CreateJittedFibonacci) {
    // maybe the module and shouldnt be owned by the jit?:?
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    CreateFibFunction(mcJit.getModuleNonOwning());
//    std::cout << mcJit.emitToString();
    fibonacciFnPtr fibPtr = (int (*)(int)) mcJit.getFunctionAddress("fib");
    ASSERT_EQ(fibPtr(4), 3);
}

TEST_F(MCJitTests, TransferObjectsToResources) {
    ASSERT_FALSE(true);
}

using powFnTy =     double (*)(double x, double y);
using fabsFnTy =    double (*)(double x);
using acosFnTy =    double (*)(double x);
using asinFnTy =    double (*)(double x);
using atanFnTy =    double (*)(double x);
using ceilFnTy =    double (*)(double x);
using cosFnTy =     double (*)(double x);
using coshFnTy =    double (*)(double x);
using expFnTy =     double (*)(double x);
using floorFnTy =   double (*)(double x);
using logFnTy =     double (*)(double x);
using log10FnTy =   double (*)(double x);
using sinFnTy =     double (*)(double x);
using sinhFnTy =    double (*)(double x);
using tanFnTy =     double (*)(double x);
using tanhFnTy =    double (*)(double x);
using fmodFnTy =    double (*)(double x, double y);


// double pow(double x, double y)
TEST_F(MCJitTests, LibFuncPow) {
    double expected = 16;
    std::string funcName = "pow";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    powFnTy fnPtr = (powFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(2, 4));
}


TEST_F(MCJitTests, CheckLibFuncFabs){
    double expected = 5.3;
    std::string funcName = "fabs";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    fabsFnTy fnPtr = (fabsFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(-5.3));
}

TEST_F(MCJitTests, CheckLibFuncAcos){
    // Inverse of cos(-0.50) = 2.09 in radians
    double expected = 2.0943951;
    std::string funcName = "acos";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    acosFnTy fnPtr = (acosFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-3);
}

TEST_F(MCJitTests, CheckLibFuncAsin){
    // Inverse of sin(-0.50) = -0.52 in radians
    double expected = -0.523598776;
    std::string funcName = "asin";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    asinFnTy fnPtr = (asinFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncAtan){
    double expected = -0.463647609;
    std::string funcName = "atan";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    atanFnTy fnPtr = (atanFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncCeil){
    double expected = 7;
    std::string funcName = "ceil";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    ceilFnTy fnPtr = (ceilFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(6.3));
}
TEST_F(MCJitTests, CheckLibFuncCos){
    double expected = 0.87758256189;
    std::string funcName = "cos";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    cosFnTy fnPtr = (cosFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncCosh){
    double expected = 1.1276259652;
    std::string funcName = "cosh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    coshFnTy fnPtr = (coshFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncExp){
    double expected = 0.60653065971;
    std::string funcName = "exp";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    expFnTy fnPtr = (expFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncFloor){
    double expected = 3;
    std::string funcName = "floor";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    floorFnTy fnPtr = (floorFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(3.7));
}
TEST_F(MCJitTests, CheckLibFuncLog){
    double expected = 3.4657;
    std::string funcName = "log";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    logFnTy fnPtr = (logFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(32), 1e-3);
}
TEST_F(MCJitTests, CheckLibFuncLog10){
    double expected = 2;
    std::string funcName = "log10";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    log10FnTy fnPtr = (log10FnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(100), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncSin){
    double expected = -0.4794255386;
    std::string funcName = "sin";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    sinFnTy fnPtr = (sinFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncSinh){
    double expected = -0.52109530549;
    std::string funcName = "sinh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    sinhFnTy fnPtr = (sinhFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncTan){
    double expected = -0.54630248984;
    std::string funcName = "tan";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    tanFnTy fnPtr = (tanFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncTanh){
    double expected = -0.46211715726;
    std::string funcName = "tanh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    tanhFnTy fnPtr = (tanhFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}
TEST_F(MCJitTests, CheckLibFuncFmod){
    double expected = 1;
    std::string funcName = "fmod";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    fmodFnTy fnPtr = (fmodFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(5, 2));
}












