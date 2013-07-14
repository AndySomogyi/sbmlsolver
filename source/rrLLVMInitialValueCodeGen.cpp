/*
 * rrLLVMInitialValueCodeGen.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#include "rrLLVMInitialValueCodeGen.h"
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

LLVMInitialValueCodeGen::LLVMInitialValueCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        initialValuesFunc(0)
{
    model->getListOfSpecies()->accept(*this);
    model->getListOfCompartments()->accept(*this);
    model->getListOfParameters()->accept(*this);
    model->getListOfRules()->accept(*this);
    model->getListOfReactions()->accept(*this);
    model->getListOfInitialAssignments()->accept(*this);
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

    cout << "reactions: ";
    vector<string> ids = symbols.getReactionIds();
    for (int i = 0; i < ids.size(); i++)
    {
        cout << ids[i] << ", ";
    }
    cout << "\n";
    for (int i = 0; i < stoichRowIdx.size(); i++)
    {
        cout << "row: " << stoichRowIdx[i] << ", col: " << stoichColIdx[i]
                << ", stoich: ";
        char* formula = SBML_formulaToString(stoichNodes[i]);
        cout << formula << "\n";
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
    const SpeciesReference *reference = 0;

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
    else if ((reference = dynamic_cast<const SpeciesReference*>(element)))
    {
        processSpeciesReference(reference, math);
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

bool LLVMInitialValueCodeGen::visit(const libsbml::Reaction& r)
{
    const ListOfSpeciesReferences *reactants = r.getListOfReactants();
    const ListOfSpeciesReferences *products = r.getListOfProducts();

    for (int i = 0; i < reactants->size(); i++)
    {
        processSpeciesReference(
                dynamic_cast<const SpeciesReference*>(reactants->get(i)), &r,
                Reactant, 0);
    }

    for (int i = 0; i < products->size(); i++)
    {
        processSpeciesReference(
                dynamic_cast<const SpeciesReference*>(products->get(i)), &r,
                Product, 0);
    }

    /*
     const ListOf *list = dynamic_cast<const ListOf *>(sr.getParentSBMLObject());
     const Reaction *r = dynamic_cast<const Reaction*>(list->getParentSBMLObject());

     string speciesId = sr.getSpecies();
     string reactionId = r->getId();

     cout << "species: " << sr.getSpecies() << "\n";
     cout << "reaction: " << r->getId() << "\n";
     cout << "sr.isSetStoichiometry(): " << sr.isSetStoichiometry() << "\n";
     cout << "sr.isSetStoichiometryMath(): " << sr.isSetStoichiometryMath() << "\n";
     cout << "stoichiometry: " << sr.getStoichiometry() << "\n";
     */

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

void LLVMInitialValueCodeGen::processSpeciesReference(
        const libsbml::SpeciesReference* sr, const ASTNode* math)
{
    SpeciesReferenceType type;

    const ListOf *list = dynamic_cast<const ListOf *>(sr->getParentSBMLObject());
    const Reaction *r = dynamic_cast<const Reaction*>(list->getParentSBMLObject());

    if (list == r->getListOfReactants()) {
        type = Reactant;
    }
    else if (list == r->getListOfProducts()) {
        type = Product;
    }
    else {
        string err = "could not determine if species reference ";
        err += sr->getId();
        err += " is a reactant or product";
        throw LLVMException(err, __FUNC__);
    }

    processSpeciesReference(sr, r, type, math);
}

void LLVMInitialValueCodeGen::processSpeciesReference(
        const libsbml::SpeciesReference* ref, const libsbml::Reaction* reaction,
        SpeciesReferenceType type, const ASTNode* stoich)
{
    // we might not have a floating species for this reference,
    // that should probably be an error, but there are many
    // cases in the test suite that have this, so just ignore
    // the ref in such cases.
    try
    {
        int rowIdx = symbols.getFloatingSpeciesIndex(ref->getSpecies());
        int colIdx = symbols.getReactionIndex(reaction->getId());

        if (stoich == 0)
        {
            if (ref->isSetStoichiometryMath()
                    && ref->getStoichiometryMath()->isSetMath())
            {
                stoich = ref->getStoichiometryMath()->getMath();
            }
            else
            {
                ASTNode *m = nodes.create(AST_REAL);
                m->setValue(ref->getStoichiometry());
                stoich = m;
            }
        }

        if (type == Reactant)
        {
            ASTNode *mul = nodes.create(AST_TIMES);
            ASTNode *negone = new ASTNode(AST_REAL);
            negone->setValue(-1);
            mul->addChild(negone);
            mul->addChild(new ASTNode(*stoich));
            stoich = mul;
        }

        stoichRowIdx.push_back(rowIdx);
        stoichColIdx.push_back(colIdx);
        stoichNodes.push_back(stoich);

    } catch (LLVMException &)
    {
        string err = "could not find product ";
        err += ref->getSpecies();
        err += " in the list of floating species for reaction ";
        err += ref->getId();
        err += ", this species will be ignored in this reaction.";
        Log(lWarning) << err;
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
