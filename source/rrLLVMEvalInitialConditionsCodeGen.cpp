/*
 * rrLLVMInitialValueCodeGen.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#include "rrLLVMEvalInitialConditionsCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>


using namespace libsbml;
using namespace llvm;
using namespace std;


namespace rr
{

const char* LLVMEvalInitialConditionsCodeGen::FunctionName = "evalInitialConditions";

LLVMEvalInitialConditionsCodeGen::LLVMEvalInitialConditionsCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        initialValuesFunc(0)
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

    Log(lInfo) << "globalParameters: \n";
    for (LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialValues().globalParameters.begin();
            i != modelSymbols.getInitialValues().globalParameters.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        Log(lInfo) << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }



    Log(lInfo) << "reactions: ";
    vector<string> ids = dataSymbols.getReactionIds();
    for (int i = 0; i < ids.size(); i++)
    {
        Log(lInfo) << ids[i] << ", ";
    }
    Log(lInfo) << "\n";


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

    Argument &arg = initialValuesFunc->getArgumentList().front();

    Value *argVal = &arg;


    LLVMModelDataIRBuilder modelDataBuilder(modelData, dataSymbols, builder);

    codeGenFloatingSpecies(modelData, modelDataBuilder);

    codeGenCompartments(modelData, modelDataBuilder);

    codeGenStoichiometry(modelData, modelDataBuilder);


    builder.CreateRetVoid();

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (verifyFunction(*initialValuesFunc, PrintMessageAction))
    {
        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    initialValuesFunc->dump();


    return initialValuesFunc;
}


llvm::Value* LLVMEvalInitialConditionsCodeGen::symbolValue(const std::string& symbol)
{
    LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialAssigments().find(symbol);
    if (i != modelSymbols.getInitialAssigments().end())
    {
        return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
    }

    i = modelSymbols.getAssigmentRules().find(symbol);
    if (i != modelSymbols.getAssigmentRules().end())
    {
        return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
    }

    i = modelSymbols.getInitialValues().find(symbol);
    if (i != modelSymbols.getInitialValues().end())
    {
        return LLVMASTNodeCodeGen(builder, *this).codeGen(i->second);
    }

    else
    {
        string msg = "Could not find requested symbol \'";
        msg += symbol;
        msg += "\' in symbol forest";
        throw LLVMException(msg, __FUNC__);
    }
}


void LLVMEvalInitialConditionsCodeGen::codeGenFloatingSpecies(
        Value *modelData, LLVMModelDataIRBuilder& modelDataBuilder)
{
    LLVMASTNodeCodeGen astCodeGen(builder, *this);

    Log(lInfo) << "floatingSpecies: \n";
    for (LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialValues().floatingSpecies.begin();
            i != modelSymbols.getInitialValues().floatingSpecies.end(); i++)
    {
        Log(lInfo) << "id: " << i->first << "\n";

        char* formula = SBML_formulaToString(i->second);
        Log(lInfo) << "\t" << i->first << ": " << formula << "\n";
        free(formula);

        Value *value = astCodeGen.codeGen(i->second);
        value->dump();

        Value *amt = 0;

        Species *species = const_cast<Model*>(model)->getListOfSpecies()->get(i->first);
        if (species->getHasOnlySubstanceUnits())
        {
            // interpret the evaluated value as an amount
            amt = value;
        }
        else
        {
            // interpret the evaluated value as a concentration.
            const ASTNode *compAST =
                    modelSymbols.getInitialValues().compartments.find(species->getCompartment())->second;
            Value *compValue = astCodeGen.codeGen(compAST);
            amt = builder.CreateFMul(value, compValue, "amt");
        }

        modelDataBuilder.createFloatSpeciesAmtStore(modelData, i->first, amt);
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
    LLVMASTNodeCodeGen astCodeGen(builder, *this);

    Log(lInfo) << "reactions: ";
    vector<string> ids = dataSymbols.getReactionIds();
    for (int i = 0; i < ids.size(); i++)
    {
        Log(lInfo) << ids[i] << ", ";
    }
    Log(lInfo) << "\n";

    Value *stoichEP = modelDataBuilder.createGEP(Stoichiometry);
    Value *stoich = builder.CreateLoad(stoichEP, "stoichiometry");

    list<pair<int,int> > stoichEntries = dataSymbols.getStoichiometryIndx();
    for (list<pair<int,int> >::iterator i = stoichEntries.begin();
            i != stoichEntries.end(); i++)
    {
        pair<int, int> nz = *i;
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
    LLVMASTNodeCodeGen astCodeGen(builder, *this);

    Log(lInfo) << "compartments: \n";
    for (LLVMSymbolForest::ConstIterator i =
            modelSymbols.getInitialValues().compartments.begin();
            i != modelSymbols.getInitialValues().compartments.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        Log(lInfo) << "\t" << i->first << ": " << formula << "\n";
        free(formula);

        Value *value = astCodeGen.codeGen(i->second);
        value->dump();

        modelDataBuilder.createCompStore(i->first, value);
    }
}

} /* namespace rr */
