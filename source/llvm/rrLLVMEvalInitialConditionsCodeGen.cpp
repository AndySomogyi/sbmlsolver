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
        LLVMCodeGenBase<LLVMEvalInitialConditions_FunctionPtr>(mgc),
        initialValueResolver(model, dataSymbols, modelSymbols, builder)
{
}

LLVMEvalInitialConditionsCodeGen::~LLVMEvalInitialConditionsCodeGen()
{
}

Value* LLVMEvalInitialConditionsCodeGen::codeGen()
{
    Value *modelData = 0;

    BasicBlock *basicBlock = codeGenVoidModelDataHeader(FunctionName, modelData);

    Log(lInfo) << "boundarySpecies: \n";
    for (LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialValues().boundarySpecies.begin();
            i != modelSymbols.getInitialValues().boundarySpecies.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        Log(lInfo) << "\t" << i->first << ": " << formula << "\n";
        free(formula);
    }

    LLVMModelDataStoreSymbolResolver modelDataResolver(modelData, model,
            modelSymbols, dataSymbols, builder, initialValueResolver);

    codeGenSpecies(modelDataResolver);

    codeGenCompartments(modelDataResolver);

    codeGenStoichiometry(modelData, modelDataResolver);

    codeGenParameters(modelDataResolver);

    builder.CreateRetVoid();

    return verifyFunction();
}

void LLVMEvalInitialConditionsCodeGen::codeGenSpecies(
        LLVMModelDataStoreSymbolResolver& modelDataResolver)
{
    {
        vector<string> floatingSpecies = dataSymbols.getFloatingSpeciesIds();

        for (vector<string>::const_iterator i = floatingSpecies.begin();
                i != floatingSpecies.end(); i++)
        {
            const string& id = *i;

            if (!dataSymbols.hasAssignmentRule(id))
            {
                modelDataResolver.storeSymbolValue(id,
                        initialValueResolver.loadSymbolValue(id));
            }
        }
    }

    {
        vector<string> boundarySpecies = dataSymbols.getBoundarySpeciesIds();

        for (vector<string>::const_iterator i = boundarySpecies.begin();
                i != boundarySpecies.end(); i++)
        {
            const string& id = *i;

            if (!dataSymbols.hasAssignmentRule(id))
            {
                modelDataResolver.storeSymbolValue(id,
                        initialValueResolver.loadSymbolValue(id));
            }
        }
    }
}


void LLVMEvalInitialConditionsCodeGen::codeGenStoichiometry(
        llvm::Value *modelData, LLVMModelDataStoreSymbolResolver& modelDataResolver)
{
    LLVMModelDataIRBuilder modelDataBuilder(modelData, dataSymbols,
                builder);
    LLVMASTNodeCodeGen astCodeGen(builder, initialValueResolver);

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
        LLVMModelDataStoreSymbolResolver& modelDataResolver)
{
    vector<string> compartments = dataSymbols.getCompartmentIds();

    for (vector<string>::const_iterator i = compartments.begin();
            i != compartments.end(); i++)
    {
        const string& id = *i;

        if (!dataSymbols.hasAssignmentRule(id))
        {
            modelDataResolver.storeSymbolValue(id,
                    initialValueResolver.loadSymbolValue(id));
        }
    }
}

void LLVMEvalInitialConditionsCodeGen::codeGenParameters(
        LLVMModelDataStoreSymbolResolver& modelDataResolver)
{
    vector<string> globalParameters = dataSymbols.getGlobalParameterIds();

    for (vector<string>::const_iterator i = globalParameters.begin();
            i != globalParameters.end(); i++)
    {
        const string& id = *i;

        if (!dataSymbols.hasAssignmentRule(id))
        {
            modelDataResolver.storeSymbolValue(id,
                    initialValueResolver.loadSymbolValue(id));
        }
    }
}

} /* namespace rr */
