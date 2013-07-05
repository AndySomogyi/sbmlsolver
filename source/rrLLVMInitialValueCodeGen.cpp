/*
 * rrLLVMInitialValueCodeGen.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#include "rrLLVMInitialValueCodeGen.h"
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
    cout << "floatingSpeciesConcentrations: \n";
    for (LLVMSymbolForest::Iterator i =
            symbolForest.floatingSpeciesConcentrations.begin();
            i != symbolForest.floatingSpeciesConcentrations.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }

    return 0;
}

bool LLVMInitialValueCodeGen::visit(const Compartment& x)
{
    ASTNode *node = nodes.create(AST_REAL);
    node->setValue(x.getVolume());
    symbolForest.compartments[x.getId()] = node;
    return true;
}

bool LLVMInitialValueCodeGen::visit(const Species& x)
{
    ASTNode *amt = 0;
    ASTNode *conc = 0;
    ASTNode *comp = 0;

    if (x.isSetInitialAmount())
    {
        // ASTNode takes ownership of children
        conc = nodes.create(AST_DIVIDE);
        amt = new ASTNode(AST_REAL);
        amt->setValue(x.getInitialAmount());
        conc->addChild(amt);
        comp = new ASTNode(AST_NAME);
        comp->setName(x.getCompartment().c_str());
        conc->addChild(comp);
    }

    else if (x.isSetInitialConcentration())
    {
        conc = new ASTNode(AST_REAL);
        conc->setValue(x.getInitialConcentration());
        amt = nodes.create(AST_TIMES);
        amt->addChild(conc);
        comp = new ASTNode(AST_NAME);
        comp->setName(x.getCompartment().c_str());
        amt->addChild(comp);
    }
    else
    {
        conc = nodes.create(AST_REAL);
        conc->setValue(0);
        amt = nodes.create(AST_REAL);
        amt->setValue(0);
    }


    if (x.getBoundaryCondition())
    {
        symbolForest.boundarySpeciesConcentrations[x.getId()] = conc;
    }
    else
    {
         symbolForest.floatingSpeciesConcentrations[x.getId()] = conc;
         symbolForest.floatingSpeciesAmounts[x.getId()] = amt;
    }

    return true;
}

bool LLVMInitialValueCodeGen::visit(const Parameter& x)
{
    ASTNode *value = nodes.create(AST_REAL);
    value->setValue(x.getValue());
    symbolForest.globalParameters[x.getId()] = value;
    return true;
}

bool LLVMInitialValueCodeGen::visit(const libsbml::AssignmentRule& x)
{
    return true;
}

bool LLVMInitialValueCodeGen::visit(const libsbml::InitialAssignment& x)
{



    return true;
}

} /* namespace rr */
