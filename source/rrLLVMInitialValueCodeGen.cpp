/*
 * rrLLVMInitialValueCodeGen.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#include "rrLLVMInitialValueCodeGen.h"
#include "rrLLVMException.h"
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

    cout << "floatingSpeciesAmounts: \n";
    for (LLVMSymbolForest::Iterator i =
            symbolForest.floatingSpeciesAmounts.begin();
            i != symbolForest.floatingSpeciesAmounts.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }

    cout << "boundarySpeciesConcentrations: \n";
    for (LLVMSymbolForest::Iterator i =
            symbolForest.boundarySpeciesConcentrations.begin();
            i != symbolForest.boundarySpeciesConcentrations.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }

    cout << "globalParameters: \n";
    for (LLVMSymbolForest::Iterator i =
            symbolForest.globalParameters.begin();
            i != symbolForest.globalParameters.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }

    cout << "compartments: \n";
    for (LLVMSymbolForest::Iterator i =
            symbolForest.compartments.begin();
            i != symbolForest.compartments.end(); i++)
    {
        char* formula = SBML_formulaToString(i->second);
        cout << "\t" << i->first << ": " << formula << "\n";
        free(formula);

    }

    return 0;
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

void LLVMInitialValueCodeGen::processSpecies(const libsbml::Species *species,
        const ASTNode* math)
{
    ASTNode *amt = 0;
    ASTNode *conc = 0;
    ASTNode *comp = 0;

    // ASTNode takes ownership of children

    if (species->isSetInitialConcentration())
    {
        if (math)
        {
            conc = new ASTNode(*math);
        }
        else
        {
            conc = new ASTNode(AST_REAL);
            conc->setValue(species->getInitialConcentration());
        }
        amt = nodes.create(AST_TIMES);
        amt->addChild(conc);
        comp = new ASTNode(AST_NAME);
        comp->setName(species->getCompartment().c_str());
        amt->addChild(comp);
    }
    else
    {
        if (math)
        {
            amt = new ASTNode(*math);
        }
        else
        {
            amt = new ASTNode(AST_REAL);
            amt->setValue(species->getInitialAmount());
        }
        conc = nodes.create(AST_DIVIDE);
        conc->addChild(amt);
        comp = new ASTNode(AST_NAME);
        comp->setName(species->getCompartment().c_str());
        conc->addChild(comp);
    }

    if (species->getBoundaryCondition())
    {
        symbolForest.boundarySpeciesConcentrations[species->getId()] = conc;
    }
    else
    {
        symbolForest.floatingSpeciesConcentrations[species->getId()] = conc;
        symbolForest.floatingSpeciesAmounts[species->getId()] = amt;
    }
}



} /* namespace rr */
