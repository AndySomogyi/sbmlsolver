/*
 * rrLLVMModelDataIRBuilder.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */
#include "rrLLVMModelDataIRBuilder.h"
#include "lsLibStructural.h"
#include "rrLLVMException.h"
#include "rrOSSpecifics.h"

#include <sbml/Model.h>
#include <sbml/SBMLDocument.h>

#include <llvm/Support/raw_ostream.h>

#include <string>
#include <vector>
#include <sstream>

#include "llvm/ADT/APInt.h"


using namespace libsbml;
using namespace llvm;
using namespace std;

namespace rr
{

static Module *TheModule;
static IRBuilder<> *builder;
static ExecutionEngine *TheExecutionEngine;
static LLVMContext *context;

static LLVMContext &getContext() {
    return *context;
}

Value *storeInStruct(IRBuilder<> *B, Value *S, Value *V, unsigned index, const char* s = "");
Value *loadFromStruct(IRBuilder<> *B, Value *S, unsigned index, const char* s = "");

struct TestStruct {
    int var0;
    double var1;
    double var2;
    char* var3;
    double var4;
    char* var5;
};

StructType* getTestStructType() {
    // static StructType *     create (ArrayRef< Type * > Elements, StringRef Name, bool isPacked=false)
    vector<Type*> elements;
    elements.push_back(Type::getInt32Ty(getContext()));   // int var0;
    elements.push_back(Type::getDoubleTy(getContext()));  // double var1;
    elements.push_back(Type::getDoubleTy(getContext()));  // double var2;
    //elements.push_back(ArrayType::get(Type::getInt8Ty(getContext()), 0)); // char* var3;
    elements.push_back(Type::getInt8PtrTy(getContext())); // char* var5;
    elements.push_back(Type::getDoubleTy(getContext()));  // double var4;
    //elements.push_back(ArrayType::get(Type::getInt8Ty(getContext()), 0));   // char[] var5;
    elements.push_back(Type::getInt8PtrTy(getContext())); // char* var5; // char* var5;




    StructType *s = StructType::create(elements, "TestStruct");

    const DataLayout &dl = *TheExecutionEngine->getDataLayout();

    size_t llvm_size = dl.getTypeStoreSize(s);

    printf("TestStruct size: %i, , LLVM Size: %i\n", sizeof(TestStruct), llvm_size);



    return s;
}

Value *getVar5(Value *testStructPtr, int index) {
    GetElementPtrInst *gep = dynamic_cast<GetElementPtrInst*>(builder->CreateStructGEP(testStructPtr, 5, "var5_gep"));
    LoadInst * var5_load = builder->CreateLoad(gep, "var5_load");
    //Value *var5_load = loadFromStruct(Builder, testStructPtr, 5, "var5");
    gep = dynamic_cast<GetElementPtrInst*>(builder->CreateConstGEP1_32(var5_load, index, "var5_elem_gep"));
    return builder->CreateLoad(gep, "var5_elem");
}


static void dispStruct(TestStruct *s)
{
    printf("%s {\n",  __PRETTY_FUNCTION__);
    printf("\tvar0: %i\n", s->var0);
    printf("\tvar1: %f\n", s->var1);
    printf("\tvar2: %f\n", s->var2);
    printf("\tvar3: %s\n", s->var3);
    printf("\tvar4: %f\n", s->var4);
    printf("\tvar5: %s\n", s->var5);
    printf("}\n");


}

static Function* dispStructProto() {

    StructType *structType = getTestStructType();
    PointerType *structTypePtr = llvm::PointerType::get(structType, 0);
    std::vector<Type*> args(1, structTypePtr);
    FunctionType *funcType = FunctionType::get(Type::getVoidTy(getContext()), args, false);
    Function *f = Function::Create(funcType, Function::InternalLinkage, "dispStruct", TheModule);
    return f;
}

static void dispInt(int i) {
    printf("%s, %i\n", __PRETTY_FUNCTION__, i);
}

static Function* dispIntProto() {
    Function *f = TheModule->getFunction("dispInt");

    if (f == 0) {
        std::vector<Type*> args(1, Type::getInt32Ty(getContext()));
        FunctionType *funcType = FunctionType::get(Type::getVoidTy(getContext()), args, false);
        f = Function::Create(funcType, Function::InternalLinkage, "dispInt", TheModule);
    }
    return f;
}

static void callDispInt(int val) {
    // Evaluate a top-level expression into an anonymous function.

    Function *func = TheExecutionEngine->FindFunctionNamed("dispInt");

    // JIT the function, returning a function pointer.
    void *fptr = TheExecutionEngine->getPointerToFunction(func);

    // Cast it to the right type (takes no arguments, returns a double) so we
    // can call it as a native function.
    void (*fp)(int) = (void (*)(int))fptr;

    fp(val);
}



static void dispDouble(double d) {
    printf("%s: %f\n", __PRETTY_FUNCTION__, d);
}

static Function* dispDoubleProto() {
    Function *f = TheModule->getFunction("dispDouble");
    if (f == 0) {
        std::vector<Type*>args(1, Type::getDoubleTy(getContext()));
        FunctionType *funcType = FunctionType::get(Type::getVoidTy(getContext()), args, false);
        f = Function::Create(funcType, Function::InternalLinkage, "dispDouble", TheModule);
    }
    return f;
}

static void dispCharStar(char* p) {
    printf("%s: %s\n", __PRETTY_FUNCTION__, p);
}

static Function* dispCharStarProto() {
    Function *f = TheModule->getFunction("dispCharStar");
    if (f == 0) {
        std::vector<Type*>args(1, Type::getInt8PtrTy(getContext()));
        FunctionType *funcType = FunctionType::get(Type::getVoidTy(getContext()), args, false);
        f = Function::Create(funcType, Function::InternalLinkage, "dispCharStar", TheModule);
    }
    return f;
}

static void dispChar(char p) {
    printf("%s as char: %c\n", __PRETTY_FUNCTION__, p);
    printf("%s as int: %i\n", __PRETTY_FUNCTION__, (int)p);
}

static Function* dispCharProto() {
    Function *f = TheModule->getFunction("dispChar");
    if (f == 0) {
        std::vector<Type*>args(1, Type::getInt8Ty(getContext()));
        FunctionType *funcType = FunctionType::get(Type::getVoidTy(getContext()), args, false);
        f = Function::Create(funcType, Function::InternalLinkage, "dispChar", TheModule);
    }
    return f;
}

static void createDispPrototypes() {
    TheExecutionEngine->addGlobalMapping(dispStructProto(), (void*)dispStruct);
    TheExecutionEngine->addGlobalMapping(dispIntProto(), (void*)dispInt);
    TheExecutionEngine->addGlobalMapping(dispDoubleProto(), (void*)dispDouble);
    TheExecutionEngine->addGlobalMapping(dispCharStarProto(), (void*)dispCharStar);
    TheExecutionEngine->addGlobalMapping(dispCharProto(), (void*)dispChar);
}

static void structSetBody(Function *func) {
    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(getContext(), "entry", func);
    builder->SetInsertPoint(BB);

    LLVMContext &context = getContext();

    std::vector<Value*> args;

    // Set names for all arguments.
    unsigned idx = 0;
    for (Function::arg_iterator ai = func->arg_begin(); ai != func->arg_end();
            ++ai, ++idx) {

        args.push_back(ai);
        //ai->setName(Args[Idx]);

        // Add arguments to variable symbol table.
        //NamedValues[Args[Idx]] = AI;
    }

    Value *arg = ConstantInt::get(Type::getInt32Ty(getContext()), 16);

    builder->CreateCall(dispIntProto(), args[1], "");

    builder->CreateCall(dispDoubleProto(), args[2], "");

    //builder->CreateCall(dispDoubleProto(), args[3], "");

    builder->CreateCall(dispCharStarProto(), args[4], "");

    storeInStruct(builder, args[0], args[1], 0);
    storeInStruct(builder, args[0], args[2], 1);
    storeInStruct(builder, args[0], args[3], 2);
    storeInStruct(builder, args[0], args[5], 4);



    Value *var3 = loadFromStruct(builder, args[0], 3, "var3");

    //Value *var3 = args[4];

    Type *t = var3->getType();

    var3->dump();
    t->dump();


    Value *c = ConstantInt::get(Type::getInt8Ty(getContext()), 's');
    Value *gep = builder->CreateConstGEP1_32(var3, 0, "array gep");
    builder->CreateStore(c, gep);
    gep = builder->CreateConstGEP1_32(var3, 1, "array gep");
    builder->CreateStore(c, gep);
    gep = builder->CreateConstGEP1_32(var3, 2, "array gep");
    builder->CreateStore(c, gep);


    c = ConstantInt::get(Type::getInt8Ty(getContext()), '2');

    Value *var5_2 = getVar5(args[0], 2);

    var5_2->dump();

    builder->CreateCall(dispCharProto(), var5_2);



    builder->CreateCall(dispCharStarProto(), args[4], "");




    // Finish off the function.
    builder->CreateRetVoid();

    // Validate the generated code, checking for consistency.
    verifyFunction(*func);
}

static Function* structSetProto() {
    Function *f = TheModule->getFunction("structSet");

    if (f == 0) {
        vector<Type*> args;
        StructType *structType = getTestStructType();
        PointerType *structTypePtr = llvm::PointerType::get(structType, 0);
        args.push_back(structTypePtr);
        args.push_back(Type::getInt32Ty(getContext()));   // int var0;
        args.push_back(Type::getDoubleTy(getContext()));  // double var1;
        args.push_back(Type::getDoubleTy(getContext()));  // double var2;
        args.push_back(Type::getInt8PtrTy(getContext())); // char* var3;
        args.push_back(Type::getDoubleTy(getContext()));  // double var4;
        args.push_back(Type::getInt8PtrTy(getContext())); // char* var5;

        FunctionType *funcType = FunctionType::get(Type::getVoidTy(getContext()), args, false);
        f = Function::Create(funcType, Function::InternalLinkage, "structSet", TheModule);


        structSetBody(f);

    }
    return f;
}

static void callStructSet(TestStruct *s, int var0, double var1, double var2,
        char* var3, double var4, char* var5)
{
    Function *func = TheExecutionEngine->FindFunctionNamed("structSet");

    // JIT the function, returning a function pointer.
    void *fptr = TheExecutionEngine->getPointerToFunction(func);

    // Cast it to the right type (takes no arguments, returns a double) so we
    // can call it as a native function.
    void (*fp)(TestStruct*, int, double, double, char*, double, char*) =
            (void (*)(TestStruct*, int, double, double,char*, double, char*))fptr;

    fp(s, var0, var1, var2, var3, var4, var5);



}



// Store V in structure S element index
Value *storeInStruct(IRBuilder<> *B, Value *S, Value *V, unsigned index, const char* s)
{
    return B->CreateStore(V, B->CreateStructGEP(S, index, s), s);
}

// Store V in structure S element index
Value *loadFromStruct(IRBuilder<> *B, Value *S, unsigned index, const char* s)
{
    Value *gep = B->CreateStructGEP(S, index, s);
    return B->CreateLoad(gep, s);
}

void dispIntrinsics(Intrinsic::ID id) {
    Intrinsic::IITDescriptor ids[8];
    SmallVector<Intrinsic::IITDescriptor, 8> table;
    Intrinsic::getIntrinsicInfoTableEntries(id, table);

    printf("table: %i\n", table.size());

    for(unsigned i = 0; i < table.size(); i++) {
        ids[i] = table[i];
    }

    for(unsigned i = 0; i < table.size(); i++) {
        Intrinsic::IITDescriptor::IITDescriptorKind kind = ids[i].Kind;
        Intrinsic::IITDescriptor::ArgKind argKind = ids[i].getArgumentKind();
        unsigned argNum = ids[i].getArgumentNumber();

        printf("kind: %i, argKind: %i, argNum: %i\n", kind, argKind, argNum);

    }

    printf("foo\n");

    std::vector<Type*> args(table.size() - 1, Type::getDoubleTy(getContext()));

    printf("intrinsic name: %s\n", getName(id, args).c_str());

    Function *decl = Intrinsic::getDeclaration(TheModule, id, args);
    decl->dump();

    FunctionType *func = Intrinsic::getType(getContext(), id, args);

    func->dump();

    printf("done\n");





    //func->dump();
}




LLVMModelDataIRBuilder::LLVMModelDataIRBuilder(const LLVMModelDataSymbols &symbols,
        IRBuilder<> *b) :
        symbols(symbols),
        builder(b),
        cachedStructValue(0)
{

}

LLVMModelDataIRBuilder::~LLVMModelDataIRBuilder()
{
    // TODO Auto-generated destructor stub
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesConcEP(llvm::Value* s,
        const std::string& id)
{
    validateStruct(s, __FUNC__);
    int index = symbols.getFloatingSpeciesIndex(id);
    return createGEP(s, FloatingSpeciesConcentrations, index);
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciedAmtEP(llvm::Value* s,
        const std::string& id)
{
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesConcFromAmtLoad(
        llvm::Value* s, const std::string& id)
{
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesAmtFromConcLoad(
        llvm::Value* s, const std::string& id)
{
}

llvm::Value* LLVMModelDataIRBuilder::createGlobalParamEP(llvm::Value* s,
        const std::string& id)
{
    validateStruct(s, __FUNC__);
    int index = symbols.getGlobalParameterIndex(id);
    return createGEP(s, GlobalParameters, index);
}


llvm::Value* LLVMModelDataIRBuilder::createGEP(llvm::Value* s,
        ModelDataFields field)
{
    validateStruct(s, __FUNC__);
    return builder->CreateStructGEP(s, (unsigned)field, "");
}

void LLVMModelDataIRBuilder::createAccessors(Module *module)
{
    const string getSizeName = "get_size";
    Function* getSizeFunc = module->getFunction(getSizeName);
    if(!getSizeFunc)
    {
        LLVMContext &context = module->getContext();
        StructType *structType = LLVMModelDataValue::getStructType(module);
        vector<Type*> getArgTypes(1, PointerType::get(structType, 0));
        FunctionType *getFuncType = FunctionType::get(Type::getInt32Ty(context),
                getArgTypes, false);
        getSizeFunc = Function::Create(getFuncType, Function::ExternalLinkage,
                getSizeName, module);

        BasicBlock *getBlock = BasicBlock::Create(context, "entry", getSizeFunc);
        builder->SetInsertPoint(getBlock);
        vector<Value*> getArgValues;
        for(Function::arg_iterator i = getSizeFunc->arg_begin();
                i != getSizeFunc->arg_end(); i++)
        {
            Value *v = i;
            //v->dump();
            getArgValues.push_back(i);
        }



        Value *gep = createGEP(getArgValues[0], Size);

        Value *getRet = builder->CreateLoad(gep);

        builder->CreateRet(getRet);

        verifyFunction(*getSizeFunc);

        //getSizeFunc->dump();

    }

}

pair<Function*, Function*> LLVMModelDataIRBuilder::createFloatingSpeciesAccessors(
        llvm::Module* module, const std::string id)
{
    const string getName = "get_floatingspecies_conc_" + id;
    const string setName = "set_floatingspecies_conc_" + id;

    pair<Function*, Function*> result(module->getFunction(getName),
            module->getFunction(setName));

    if (!result.first || !result.second)
    {
        LLVMContext &context = module->getContext();
        StructType *structType = LLVMModelDataValue::getStructType(module);
        vector<Type*> getArgTypes(1, PointerType::get(structType, 0));
        FunctionType *getFuncType = FunctionType::get(
                Type::getDoubleTy(context), getArgTypes, false);
        result.first = Function::Create(getFuncType, Function::ExternalLinkage,
                getName, module);

        BasicBlock *block = BasicBlock::Create(context, "entry",
                result.first);
        builder->SetInsertPoint(block);
        vector<Value*> getArgValues;
        for (Function::arg_iterator i = result.first->arg_begin();
                i != result.first->arg_end(); i++)
        {
            Value *v = i;
            //v->dump();
            getArgValues.push_back(i);

        }

        Value *getEP = createFloatSpeciesConcEP(getArgValues[0], id);

        Value *getRet = builder->CreateLoad(getEP);

        builder->CreateRet(getRet);

        verifyFunction(*result.first);

        //result.first->dump();

        vector<Type*> setArgTypes;
        setArgTypes.push_back(PointerType::get(structType, 0));
        setArgTypes.push_back(Type::getDoubleTy(context));
        FunctionType *setFuncType = FunctionType::get(Type::getVoidTy(context),
                setArgTypes, false);
        result.second = Function::Create(setFuncType, Function::ExternalLinkage,
                setName, module);

        block = BasicBlock::Create(context, "entry",
                result.second);
        builder->SetInsertPoint(block);
        vector<Value*> setArgValues;
        for (Function::arg_iterator i = result.second->arg_begin();
                i != result.second->arg_end(); i++)
        {
            Value *v = i;
            //v->dump();
            setArgValues.push_back(i);

        }



        Value *val = createFloatSpeciesConcStore(setArgValues[0], id, setArgValues[1]);

        builder->CreateRetVoid();

        verifyFunction(*result.second);

        //result.second->dump();

        cout << "pause...\n";

    }

    return result;
}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesConcStore(Value* s,
        const string& id, Value *conc)
{
    validateStruct(s, __FUNC__);
    int index = symbols.getFloatingSpeciesIndex(id);

    Value *compEP = createFloatSpeciesCompEP(s, id);
    Value *volume = builder->CreateLoad(compEP);
    Value *amount = builder->CreateFMul(conc, volume);

    builder->CreateStore(amount, createGEP(s, FloatingSpeciesAmounts, index));
    return builder->CreateStore(conc, createGEP(s, FloatingSpeciesConcentrations, index));

}

llvm::Value* LLVMModelDataIRBuilder::createFloatSpeciesCompEP(llvm::Value* s,
        const std::string& id)
{
    validateStruct(s, __FUNC__);
    int compIndex = symbols.getFloatingSpeciesCompartmentIndex(id);
    return createGEP(s, CompartmentVolumes, compIndex);
}

llvm::Value* LLVMModelDataIRBuilder::createGEP(llvm::Value* s,
    ModelDataFields field, unsigned index)
{
    Value *fieldGEP = createGEP(s, field);
    Value *load = builder->CreateLoad(fieldGEP);
    return builder->CreateConstGEP1_32(load, index);
}

void LLVMModelDataIRBuilder::validateStruct(llvm::Value* s, const char* funcName)
{
    if (!builder)
    {
        string err = "error in ";
        err += funcName;
        err += ", builder is null";
        throw LLVMException(err);
    }

    if (s != cachedStructValue)
    {
        PointerType *pointerType = dyn_cast<PointerType>(s->getType());

        // get the type the pointer refrerences
        Type *type = pointerType ? pointerType->getElementType() : s->getType();

        StructType *structType = dyn_cast<StructType>(type);

        if (structType)
        {
            //structType->dump();
            string nm = structType->getName();
            string sname = structType->getStructName();

            cout << "nm: " << nm << "\n";
            cout << "sname: " << sname << "\n";


            if (structType->getName().compare(LLVMModelDataSymbols::ModelDataName) == 0)
            {
                cachedStructValue = s;
                return;
            }
        }
        std::string str;
        raw_string_ostream err(str);
        err << "error in " << funcName << ", "
                << "Invalid argument type, expected " << LLVMModelDataSymbols::ModelDataName
                << ", but received ";
        type->print(err);
        throw LLVMException(err.str());
    }
}

void LLVMModelDataIRBuilder::test(Module *module, IRBuilder<> *build,
        ExecutionEngine * engine)
{
    TheModule = module;
    context = &module->getContext();
    rr::builder = build;
    TheExecutionEngine = engine;
    createDispPrototypes();

    callDispInt(23);

    structSetProto();

    TestStruct s =
    { 0 };

    s.var3 = (char*) calloc(50, sizeof(char));
    s.var5 = (char*) calloc(50, sizeof(char));

    sprintf(s.var5, "1234567890");

    char* p = (char*) calloc(50, sizeof(char));

    dispStruct(&s);

    callStructSet(&s, 314, 3.14, 2.78, p, 0, 0);

    printf("p: %s\n", p);

    dispStruct(&s);

    printf("new var5: ");
    for (int i = 0; i < 10; i++)
    {
        printf("{i:%i,c:%i}, ", i, s.var5[i]);
    }
    printf("\n");
}



} /* namespace rr */
