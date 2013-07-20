/*
 * rrLLVMEvalReactionRatesCodeGen.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#include "rrLLVMEvalReactionRatesCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>
#include <Poco/Logger.h>


using namespace libsbml;
using namespace llvm;
using namespace std;


namespace rr
{

const char* LLVMEvalReactionRatesCodeGen::FunctionName = "evalReactionRates";

LLVMEvalReactionRatesCodeGen::LLVMEvalReactionRatesCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        func(0),
        modelData(0)
{
}

LLVMEvalReactionRatesCodeGen::~LLVMEvalReactionRatesCodeGen()
{
}

Value* LLVMEvalReactionRatesCodeGen::codeGen()
{
    // make the set init value function
    vector<Type*> argTypes;
    StructType *modelDataStructType = LLVMModelDataIRBuilder::getStructType(
            module);
    PointerType *structTypePtr = llvm::PointerType::get(modelDataStructType, 0);
    argTypes.push_back(structTypePtr);

    FunctionType *funcType = FunctionType::get(Type::getVoidTy(context), argTypes,
            false);
    func = Function::Create(funcType, Function::InternalLinkage,
            FunctionName, module);


    // Create a new basic block to start insertion into.
    BasicBlock *basicBlock = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(basicBlock);

    // single argument
    modelData = func->arg_begin();


    LLVMModelDataIRBuilder mdbuilder(modelData, dataSymbols, builder);
    LLVMASTNodeCodeGen astCodeGen(builder, *this);
    LLVMASTNodeFactory nodes;

    // iterate through all of the reaction, and generate code based on thier
    // kinetic rules.

    // currently, our AST does not support assigments, so we do it
    // here.

    const ListOfReactions *reactions = model->getListOfReactions();

    for (int i = 0; i < reactions->size(); ++i)
    {
        const Reaction *r = reactions->get(i);
        const KineticLaw *kinetic = r->getKineticLaw();
        const ASTNode *math = 0;
        Value *value = 0;
        if (kinetic)
        {
            math = kinetic->getMath();
        }
        else
        {
            poco_warning(getLogger(), "Reaction " + r->getId() + " has no KineticLaw, it will be set to zero");
            ASTNode *m = nodes.create(AST_REAL);
            m->setValue(0);
        }
        value = astCodeGen.codeGen(math);
        mdbuilder.createReactionRateStore(r->getId(), value);
    }


    builder.CreateRetVoid();

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (verifyFunction(*func, PrintMessageAction))
    {
        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    func->dump();


    return func;
}

LLVMEvalReactionRatesCodeGen::FunctionPtr LLVMEvalReactionRatesCodeGen::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine.getPointerToFunction(func);
}

llvm::Value* LLVMEvalReactionRatesCodeGen::symbolValue(
        const std::string& symbol)
{
    return LLVMCodeGenBase::symbolValue(symbol, modelData);
}

} /* namespace rr */
