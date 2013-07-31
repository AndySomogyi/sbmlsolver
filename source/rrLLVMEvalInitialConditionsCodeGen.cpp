/*
 * rrLLVMInitialValueCodeGen.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#include "rrLLVMEvalInitialConditionsCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMInitialValueSymbolResolver.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>
#include <Poco/Logger.h>


using namespace libsbml;
using namespace llvm;
using namespace std;


namespace rr
{

const char* LLVMEvalInitialConditionsCodeGen::FunctionName = "evalInitialConditions";

LLVMEvalInitialConditionsCodeGen::LLVMEvalInitialConditionsCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        initialValuesFunc(0),
        symbolResolver(model, dataSymbols, modelSymbols, builder)
{
}

LLVMEvalInitialConditionsCodeGen::~LLVMEvalInitialConditionsCodeGen()
{
}

Value* LLVMEvalInitialConditionsCodeGen::codeGen()
{
    Log(lInfo) << "boundarySpecies: \n";
    for (LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialValues().boundarySpecies.begin();
            i != modelSymbols.getInitialValues().boundarySpecies.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        Log(lInfo) << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }



    // make the set init value function
    vector<Type*> argTypes;
    StructType *modelDataStructType = LLVMModelDataIRBuilder::getStructType(
            module);
    PointerType *structTypePtr = llvm::PointerType::get(modelDataStructType, 0);
    argTypes.push_back(structTypePtr);

    FunctionType *funcType = FunctionType::get(Type::getVoidTy(context), argTypes,
            false);
    initialValuesFunc = Function::Create(funcType, Function::InternalLinkage,
            FunctionName, module);


    // Create a new basic block to start insertion into.
    BasicBlock *basicBlock = BasicBlock::Create(context, "entry", initialValuesFunc);
    builder.SetInsertPoint(basicBlock);

    // single argument
    Value *modelData = initialValuesFunc->arg_begin();

    LLVMInitialValueSymbolResolver symbolResolver(model, dataSymbols,
            modelSymbols, builder);






    LLVMModelDataIRBuilder modelDataBuilder(modelData, dataSymbols, builder);

    codeGenSpecies(modelData, modelDataBuilder);

    codeGenCompartments(modelData, modelDataBuilder);

    codeGenStoichiometry(modelData, modelDataBuilder);

    codeGenParameters(modelData, modelDataBuilder);


    builder.CreateRetVoid();

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (verifyFunction(*initialValuesFunc, PrintMessageAction))
    {
        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    poco_information(getLogger(), string(FunctionName) + string(": ") +
            to_string(initialValuesFunc));

    return initialValuesFunc;
}


void LLVMEvalInitialConditionsCodeGen::codeGenSpecies(llvm::Value *modelData,
        LLVMModelDataIRBuilder& modelDataBuilder)
{
    LLVMASTNodeCodeGen astCodeGen(builder, symbolResolver);

    const ListOfSpecies *species = model->getListOfSpecies();
    for (unsigned i = 0; i < species->size(); i++)
    {
        const Species *s = species->get(i);
        Value *amt = 0;

        if (s->getHasOnlySubstanceUnits())
        {
            amt = symbolResolver.loadSymbolValue(s->getId());
        }
        else
        {
            Value *conc = symbolResolver.loadSymbolValue(s->getId());
            Value *comp = symbolResolver.loadSymbolValue(s->getCompartment());
            amt = builder.CreateFMul(conc, comp, s->getId() + "_amt");
        }

        if (s->getBoundaryCondition())
        {
            modelDataBuilder.createBoundSpeciesAmtStore(s->getId(), amt);
        }
        else
        {
            modelDataBuilder.createFloatSpeciesAmtStore(s->getId(), amt);
        }
    }
}



LLVMEvalInitialConditionsCodeGen::FunctionPtr LLVMEvalInitialConditionsCodeGen::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine.getPointerToFunction(func);
}

void LLVMEvalInitialConditionsCodeGen::codeGenStoichiometry(llvm::Value* modelData,
        LLVMModelDataIRBuilder& modelDataBuilder)
{
    LLVMASTNodeCodeGen astCodeGen(builder, symbolResolver);

    Log(lInfo) << "reactions: ";
    vector<string> ids = dataSymbols.getReactionIds();
    for (int i = 0; i < ids.size(); i++)
    {
        Log(lInfo) << ids[i] << ", ";
    }
    Log(lInfo) << "\n";

    Value *stoichEP = modelDataBuilder.createGEP(Stoichiometry);
    Value *stoich = builder.CreateLoad(stoichEP, "stoichiometry");

    list<pair<uint,uint> > stoichEntries = dataSymbols.getStoichiometryIndx();
    for (list<pair<uint,uint> >::iterator i = stoichEntries.begin();
            i != stoichEntries.end(); i++)
    {
        pair<uint, uint> nz = *i;
        const ASTNode *node = modelSymbols.createStoichiometryNode(nz.first, nz.second);
        char* formula = SBML_formulaToString(node);
        Log(lInfo) << "\t{" << nz.first << ", " << nz.second << "} : " << formula
                << "\n";
        free(formula);

        // createCSRMatrixSetNZ(llvm::Value *csrPtr, llvm::Value *row,
        // llvm::Value *col, llvm::Value *value, const char* name = 0);

        Value *stoichValue = astCodeGen.codeGen(node);

        delete node;

        Value *row = ConstantInt::get(Type::getInt32Ty(context), nz.first, true);
        Value *col = ConstantInt::get(Type::getInt32Ty(context), nz.second, true);
        LLVMModelDataIRBuilder::createCSRMatrixSetNZ(builder, stoich, row, col, stoichValue);

    }
}

void LLVMEvalInitialConditionsCodeGen::codeGenCompartments(
        llvm::Value* modelData, LLVMModelDataIRBuilder& modelDataBuilder)
{
    LLVMASTNodeCodeGen astCodeGen(builder, symbolResolver);

    Log(lInfo) << "compartments: \n";
    for (LLVMSymbolForest::ConstIterator i =
            modelSymbols.getInitialValues().compartments.begin();
            i != modelSymbols.getInitialValues().compartments.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        Log(lInfo) << "\t" << i->first << ": " << formula << "\n";
        free(formula);

        Value *value = astCodeGen.codeGen(i->second);
        modelDataBuilder.createCompStore(i->first, value);
    }
}

void LLVMEvalInitialConditionsCodeGen::codeGenParameters(llvm::Value* modelData,
        LLVMModelDataIRBuilder& modelDataBuilder)
{
    LLVMASTNodeCodeGen astCodeGen(builder, symbolResolver);

    Log(lInfo) << "globalParameters: \n";
    for (LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialValues().globalParameters.begin();
            i != modelSymbols.getInitialValues().globalParameters.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        Log(lInfo) << "\t" << i->first << ": " << formula << "\n";
        free(formula);

        Value *value = astCodeGen.codeGen(i->second);
        modelDataBuilder.createGlobalParamStore(i->first, value);
    }
}

} /* namespace rr */
