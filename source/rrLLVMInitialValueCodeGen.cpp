/*
 * rrLLVMInitialValueCodeGen.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#include "rrLLVMInitialValueCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMASTNodeCodeGen.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>


using namespace libsbml;
using namespace llvm;
using namespace std;

namespace rr
{

LLVMInitialValueCodeGen::LLVMInitialValueCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        initialValuesFunc(0)
{
    model->getListOfSpecies()->accept(*this);
    model->getListOfCompartments()->accept(*this);
    model->getListOfParameters()->accept(*this);
    model->getListOfInitialAssignments()->accept(*this);
    model->getListOfRules()->accept(*this);
}

LLVMInitialValueCodeGen::~LLVMInitialValueCodeGen()
{
}

Value* LLVMInitialValueCodeGen::codeGen()
{


    cout << "boundarySpecies: \n";
    for (LLVMSymbolForest::Iterator i = symbolForest.boundarySpecies.begin();
            i != symbolForest.boundarySpecies.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }

    cout << "globalParameters: \n";
    for (LLVMSymbolForest::Iterator i = symbolForest.globalParameters.begin();
            i != symbolForest.globalParameters.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }

    cout << "compartments: \n";
    for (LLVMSymbolForest::Iterator i = symbolForest.compartments.begin();
            i != symbolForest.compartments.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
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
            "structSet", module);


    // Create a new basic block to start insertion into.
    BasicBlock *basicBlock = BasicBlock::Create(context, "entry", initialValuesFunc);
    builder->SetInsertPoint(basicBlock);

    // single argument
    Value *modelData = initialValuesFunc->arg_begin();

    Argument &arg = initialValuesFunc->getArgumentList().front();

    Value *argVal = &arg;

    printf("modelData: %p\n", modelData);
    printf("arg: %p\n", argVal);

    LLVMModelDataIRBuilder modelDataBuilder(symbols, builder);
    LLVMASTNodeCodeGen astCodeGen(*builder, *this);

    cout << "floatingSpecies: \n";
    for (LLVMSymbolForest::Iterator i = symbolForest.floatingSpecies.begin();
            i != symbolForest.floatingSpecies.end(); i++)
    {
        cout << "id: " << i->first << "\n";

        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

        Value *value = astCodeGen.codeGen(i->second);
        value->dump();

        Value *amt = 0;

        Species *species = model->getListOfSpecies()->get(i->first);
        if (species->getHasOnlySubstanceUnits())
        {
            // interpret the evaluated value as an amount
            amt = value;
        }
        else
        {
            // interpret the evaluated value as a concentration.
            const ASTNode *compAST = symbolForest.compartments[species->getCompartment()];
            Value *compValue = astCodeGen.codeGen(compAST);
            amt = builder->CreateFMul(value, compValue, "amt");
        }

        modelDataBuilder.createFloatSpeciesAmtStore(modelData, i->first, amt);
    }

    builder->CreateRetVoid();

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (verifyFunction(*initialValuesFunc, PrintMessageAction))
    {
        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    initialValuesFunc->dump();


    return initialValuesFunc;
}

bool LLVMInitialValueCodeGen::visit(const libsbml::Compartment& x)
{
    ASTNode *node = nodes.create(AST_REAL);
    node->setValue(x.getVolume());
    symbolForest.compartments[x.getId()] = node;
    return true;
}

bool LLVMInitialValueCodeGen::visit(const libsbml::Species& x)
{
    processSpecies(&x, 0);
    return true;
}

bool LLVMInitialValueCodeGen::visit(const libsbml::Parameter& x)
{
    ASTNode *value = nodes.create(AST_REAL);
    value->setValue(x.getValue());
    symbolForest.globalParameters[x.getId()] = value;
    return true;
}

bool LLVMInitialValueCodeGen::visit(const libsbml::AssignmentRule& x)
{
    cout << __FUNC__ << ", id: " << x.getId() << "\n";
    SBase *element = model->getElementBySId(x.getVariable());
    processElement(element, x.getMath());
    return true;
}

bool LLVMInitialValueCodeGen::visit(const libsbml::InitialAssignment& x)
{
    cout << __FUNC__ << ", id: " << x.getId() << "\n";
    SBase *element = model->getElementBySId(x.getSymbol());
    processElement(element, x.getMath());
    return true;
}

void LLVMInitialValueCodeGen::processElement(const libsbml::SBase *element,
        const ASTNode* math)
{
    const Compartment *comp = 0;
    const Parameter *param = 0;
    const Species *species = 0;

    if ((comp = dynamic_cast<const Compartment*>(element)))
    {
        symbolForest.compartments[comp->getId()] = math;
    }
    else if ((param = dynamic_cast<const Parameter*>(element)))
    {
        symbolForest.globalParameters[param->getId()] = math;
    }
    else if ((species = dynamic_cast<const Species*>(element)))
    {
        processSpecies(species, math);
    }
    else
    {
        cout << __FUNC__ << ", WARNING, unknown element type in "
                << model->getSBMLDocument()->getName() << "\n";
    }
}

bool LLVMInitialValueCodeGen::visit(const libsbml::Rule& x)
{
    cout << __FUNC__ << "\n";
    return true;
}

void LLVMInitialValueCodeGen::processSpecies(const libsbml::Species *species,
        const ASTNode* math)
{
    // ASTNode takes ownership of children, so only allocate the ones that
    // are NOT given to an ASTNode addChild.

    if (!math)
    {
        if (species->getHasOnlySubstanceUnits())
        {
            // value is interpreted as an amount
            ASTNode *amt = 0;
            if (species->isSetInitialConcentration())
            {
                ASTNode *conc = new ASTNode(AST_REAL);
                conc->setValue(species->getInitialConcentration());

                amt = nodes.create(AST_TIMES);
                amt->addChild(conc);
                ASTNode *comp = new ASTNode(AST_NAME);
                comp->setName(species->getCompartment().c_str());
                amt->addChild(comp);
                math = amt;
            }
            else if (species->isSetInitialAmount())
            {
                amt = new ASTNode(AST_REAL);
                amt->setValue(species->getInitialAmount());
            }
            math = amt;
        }
        else
        {
            // value is interpreted as a concentration
            ASTNode *conc = 0;
            if (species->isSetInitialConcentration())
            {
                conc = new ASTNode(AST_REAL);
                conc->setValue(species->getInitialConcentration());

            }
            else if (species->isSetInitialAmount())
            {
                ASTNode *amt = new ASTNode(AST_REAL);
                amt->setValue(species->getInitialAmount());

                conc = nodes.create(AST_DIVIDE);
                conc->addChild(amt);
                ASTNode *comp = new ASTNode(AST_NAME);
                comp->setName(species->getCompartment().c_str());
                conc->addChild(comp);
            }
            math = conc;
        }
    }

    if (species->getBoundaryCondition())
    {
        symbolForest.boundarySpecies[species->getId()] = math;
    }
    else
    {
        symbolForest.floatingSpecies[species->getId()] = math;
    }
}

llvm::Value* LLVMInitialValueCodeGen::symbolValue(const std::string& symbol)
{
    LLVMSymbolForest::ConstIterator i = symbolForest.find(symbol);
    if (i != symbolForest.end())
    {
        return LLVMASTNodeCodeGen(*builder, *this).codeGen(i->second);
    }
    else
    {
        string msg = "Could not find requested symbol \'";
        msg += symbol;
        msg += "\' in symbol forest";
        throw LLVMException(msg, __FUNC__);
    }
}

} /* namespace rr */
