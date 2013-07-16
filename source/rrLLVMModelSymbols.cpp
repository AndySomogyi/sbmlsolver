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


void test (ASTNode *& a) {
    a = new ASTNode();

    const ASTNode *b = new ASTNode();

    std::vector<const ASTNode **> nodes;
    nodes.push_back(&b);

}

namespace rr
{

LLVMModelSymbols::LLVMModelSymbols(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        reactions(symbols.getReactionSize(), ReactionSymbols()),
        engine(mgc.getExecutionEngine())
{
    model->getListOfSpecies()->accept(*this);
    model->getListOfCompartments()->accept(*this);
    model->getListOfParameters()->accept(*this);
    model->getListOfRules()->accept(*this);
    model->getListOfReactions()->accept(*this);
    model->getListOfInitialAssignments()->accept(*this);
}

LLVMModelSymbols::~LLVMModelSymbols()
{
}

Value* LLVMModelSymbols::codeGen()
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

    list<pair<int,int> > stoichEntries = symbols.getStoichiometryIndx();
    for (list<pair<int,int> >::iterator i = stoichEntries.begin();
            i != stoichEntries.end(); i++)
    {
        pair<int, int> nz = *i;
        const ASTNode *node = createStoichiometryNode(nz.first, nz.second);
        char* formula = SBML_formulaToString(node);
        cout << "\t{" << nz.first << ", " << nz.second << "} : " << formula
                << "\n";
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
    builder->SetInsertPoint(basicBlock);

    // single argument
    Value *modelData = initialValuesFunc->arg_begin();

    Argument &arg = initialValuesFunc->getArgumentList().front();

    Value *argVal = &arg;

    printf("modelData: %p\n", modelData);
    printf("arg: %p\n", argVal);

    LLVMModelDataIRBuilder modelDataBuilder(symbols, builder);

    codeGenFloatingSpecies(modelData, modelDataBuilder);

    codeGenStoichiometry(modelData, modelDataBuilder);


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

bool LLVMModelSymbols::visit(const libsbml::Compartment& x)
{
    ASTNode *node = nodes.create(AST_REAL);
    node->setValue(x.getVolume());
    symbolForest.compartments[x.getId()] = node;
    return true;
}

bool LLVMModelSymbols::visit(const libsbml::Species& x)
{
    processSpecies(&x, 0);
    return true;
}

bool LLVMModelSymbols::visit(const libsbml::Parameter& x)
{
    ASTNode *value = nodes.create(AST_REAL);
    value->setValue(x.getValue());
    symbolForest.globalParameters[x.getId()] = value;
    return true;
}

bool LLVMModelSymbols::visit(const libsbml::AssignmentRule& x)
{
    cout << __FUNC__ << ", id: " << x.getId() << "\n";
    SBase *element = model->getElementBySId(x.getVariable());
    processElement(element, x.getMath());
    return true;
}

bool LLVMModelSymbols::visit(const libsbml::InitialAssignment& x)
{
    cout << __FUNC__ << ", id: " << x.getId() << "\n";
    SBase *element = model->getElementBySId(x.getSymbol());
    processElement(element, x.getMath());
    return true;
}

void LLVMModelSymbols::processElement(const libsbml::SBase *element,
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

bool LLVMModelSymbols::visit(const libsbml::Rule& x)
{
    cout << __FUNC__ << "\n";
    return true;
}

bool LLVMModelSymbols::visit(const libsbml::Reaction& r)
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



void LLVMModelSymbols::processSpecies(const libsbml::Species *species,
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

void LLVMModelSymbols::processSpeciesReference(
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

void LLVMModelSymbols::processSpeciesReference(
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

        else
        {
            cout << "stoich\n";
        }

        assert(stoich != 0);

        ReactionSymbols &reacSym = reactions[colIdx];

        // check if we already have a species ref with this id, if so
        // replace it, if not, add it

        if (type == Reactant)
        {
            StringIntMap::iterator i;
            if (ref->isSetId() &&
                    (i = reacSym.reactantRefIds.find(ref->getId())) !=
                            reacSym.reactantRefIds.end())
            {
                reacSym.nodes[i->second] = stoich;
            }
            else
            {
                reacSym.nodes.push_back(stoich);
                int stoichIdx = reacSym.nodes.size()-1; // index of new node
                reacSym.reactantIdx[rowIdx].push_back(stoichIdx);
                if (ref->isSetId())
                {
                    reacSym.reactantRefIds[ref->getId()] = stoichIdx;
                }
            }
        }
        else
        {
            StringIntMap::iterator i;
            if (ref->isSetId() &&
                    (i = reacSym.productRefIds.find(ref->getId())) !=
                            reacSym.productRefIds.end())
            {
                reacSym.nodes[i->second] = stoich;
            }
            else
            {
                reacSym.nodes.push_back(stoich);
                int stoichIdx = reacSym.nodes.size()-1; // index of new node
                reacSym.productIdx[rowIdx].push_back(stoichIdx);
                if (ref->isSetId())
                {
                    reacSym.productRefIds[ref->getId()] = stoichIdx;
                }
            }
        }
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

llvm::Value* LLVMModelSymbols::symbolValue(const std::string& symbol)
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

const ASTNode* LLVMModelSymbols::createStoichiometryNode(int row,
        int col)
{
    ReactionSymbols &r = reactions[col];
    IntList productList;
    IntList reactantList;

    IntIntListMap::const_iterator pi = r.productIdx.find(row);
    if (pi != r.productIdx.end())
    {
        productList = pi->second;
    }

    IntIntListMap::const_iterator ri = r.reactantIdx.find(row);
    if (ri != r.reactantIdx.end())
    {
        reactantList = ri->second;
    }

    if (productList.size() == 0 && reactantList.size() == 0)
    {
        string err = "species " + symbols.getFloatingSpeciesIds()[row] +
                " has neither products nor reactants in reaction " +
                symbols.getReactionIds()[col];
        throw LLVMException(err, __FUNC__);
    }

    // we keep track of the top level node, it takes ownership
    // of all child nodes.
    ASTNode *result = nodes.create(AST_PLUS);
    ASTNode *reactants = 0;
    ASTNode *products = 0;

    cout << "\t{" << row << ", " << col << "}, #reactants: " << reactantList.size() << " #products: " << productList.size() << "\n";

    if (reactantList.size())
    {
        // list is nearly always length 1, so don't waste
        // time making a plus out of it...
        if (reactantList.size() == 1)
        {
            reactants = new ASTNode(*r.nodes[reactantList.front()]);
        }
        else
        {
            reactants = new ASTNode(AST_PLUS);
            for (IntList::const_iterator i = reactantList.begin();
                    i != reactantList.end(); i++)
            {
                const ASTNode *reactant = r.nodes[*i];
                reactants->addChild(new ASTNode(*reactant));
            }
        }

        ASTNode *negOne = new ASTNode(AST_REAL);
        negOne->setValue(-1.);

        ASTNode *times = new ASTNode(AST_TIMES);
        times->addChild(negOne);
        times->addChild(reactants);

        reactants = times;
    }
    else
    {
        reactants = new ASTNode(AST_REAL);
        reactants->setValue(0.);
    }

    if (productList.size())
    {
        if (productList.size() == 1)
        {
            products = new ASTNode(*r.nodes[productList.front()]);
        }
        else
        {
            products = new ASTNode(AST_PLUS);
            for (IntList::const_iterator i = productList.begin();
                    i != productList.end(); i++)
            {
                const ASTNode *product = r.nodes[*i];
                products->addChild(new ASTNode(*product));
            }
        }
    }
    else
    {
        products = new ASTNode(AST_REAL);
        products->setValue(0.);
    }

    result->addChild(reactants);
    result->addChild(products);

    return result;
}

void LLVMModelSymbols::codeGenFloatingSpecies(
        Value *modelData, LLVMModelDataIRBuilder& modelDataBuilder)
{
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
            const ASTNode *compAST =
                    symbolForest.compartments[species->getCompartment()];
            Value *compValue = astCodeGen.codeGen(compAST);
            amt = builder->CreateFMul(value, compValue, "amt");
        }

        modelDataBuilder.createFloatSpeciesAmtStore(modelData, i->first, amt);
    }
}

LLVMModelSymbols::FunctionPtr LLVMModelSymbols::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine->getPointerToFunction(func);
}

void LLVMModelSymbols::codeGenStoichiometry(llvm::Value* modelData,
        LLVMModelDataIRBuilder& modelDataBuilder)
{
    LLVMASTNodeCodeGen astCodeGen(*builder, *this);

    cout << "reactions: ";
    vector<string> ids = symbols.getReactionIds();
    for (int i = 0; i < ids.size(); i++)
    {
        cout << ids[i] << ", ";
    }
    cout << "\n";

    Value *stoichEP = modelDataBuilder.createGEP(modelData, Stoichiometry);
    Value *stoich = builder->CreateLoad(stoichEP, "stoichiometry");

    list<pair<int,int> > stoichEntries = symbols.getStoichiometryIndx();
    for (list<pair<int,int> >::iterator i = stoichEntries.begin();
            i != stoichEntries.end(); i++)
    {
        pair<int, int> nz = *i;
        const ASTNode *node = createStoichiometryNode(nz.first, nz.second);
        char* formula = SBML_formulaToString(node);
        cout << "\t{" << nz.first << ", " << nz.second << "} : " << formula
                << "\n";
        free(formula);

        // createCSRMatrixSetNZ(llvm::Value *csrPtr, llvm::Value *row,
        // llvm::Value *col, llvm::Value *value, const char* name = 0);

        Value *stoichValue = astCodeGen.codeGen(node);
        Value *row = ConstantInt::get(Type::getInt32Ty(context), nz.first, true);
        Value *col = ConstantInt::get(Type::getInt32Ty(context), nz.second, true);
        modelDataBuilder.createCSRMatrixSetNZ(stoich, row, col, stoichValue);

    }
}

} /* namespace rr */
