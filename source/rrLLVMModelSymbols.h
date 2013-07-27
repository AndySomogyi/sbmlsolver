/*
 * rrLLVMModelSymbols.h
 *
 *  Created on: Jul 16, 2013
 *      Author: andy
 */

#ifndef LLVMLLVMModelSymbolsH
#define LLVMLLVMModelSymbolsH

#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include <sbml/Model.h>
#include <sbml/SBMLVisitor.h>

namespace rr
{

/**
 * Hold all the un-evaluated symbolic inforamtion in the model.
 *
 * TODO: some real docs...
 */
class LLVMModelSymbols: private libsbml::SBMLVisitor
{
    using libsbml::SBMLVisitor::visit;

public:
    LLVMModelSymbols(libsbml::Model const *m, LLVMModelDataSymbols const &sym);

    ~LLVMModelSymbols();

    /**
     * create an ASTNode for the species id / reaction id pair.
     *
     * This assembles the mess of items stored in the reactions array.
     */
    libsbml::ASTNode *createStoichiometryNode(int row, int col) const;

    const LLVMSymbolForest& getAssigmentRules() const;

    const LLVMSymbolForest& getInitialValues() const;

protected:

    enum SpeciesReferenceType {
        Reactant, Product
    };

    virtual bool visit(const libsbml::Compartment &x);
    virtual bool visit(const libsbml::Species &x);
    virtual bool visit(const libsbml::Parameter &x);
    virtual bool visit(const libsbml::AssignmentRule  &x);

    /**
     * InitialAssignments override the initial value specified in the element
     * definition.
     *
     * The actions of all InitialAssignment objects are in general terms the same,
     * but differ in the precise details depending on the type of variable being set:
     *
     * * In the case of a species, an InitialAssignment sets the referenced species
     * initial quantity (concentration or amount) to the value determined by the
     * formula in math. The unit associated with the value produced by the math
     * formula should be equal to the unit associated with the species quantity.
     *
     * * In the case of a species reference, an InitialAssignment sets the initial
     * stoichiometry of the reactant or product referenced by the SpeciesReference
     * object to the value determined by the formula in math. The unit associated
     * with the value produced by the math formula should be consistent with the
     * unit dimensionless, because reactant and product stoichiometries in reactions
     * are dimensionless quantities.
     *
     * * In the case of a compartment, an InitialAssignment sets the referenced
     * compartment's initial size to the size determined by the formula in math.
     * The unit associated with the value produced by the math formula should be
     * the same as that specified for the compartment's size.
     *
     * * In the case of a parameter, an InitialAssignment sets the parameter's
     * initial value to the value of the formula in math. The unit associated
     * with the value produced by the math formula should be the same as parameter's
     * units attribute value.
     */
    virtual bool visit(const libsbml::InitialAssignment &x);

    /**
     * visit the reactions so we can get all the SpeciesReferences and stuff
     * them in the initialConditions map.
     */
    virtual bool visit(const libsbml::Reaction  &x);

    /**
     * tell the acceptor to process all rules, even the ones
     * we don't handle so the iteration continues over all rules.
     *
     * The left-hand side (the variable attribute) of an assignment rule can
     * refer to the identifier of a Species, SpeciesReference, Compartment,
     * or global Parameter object in the model (but not a reaction)
     */
    virtual bool visit(const libsbml::Rule &x);

    /**
     * The only differences in how initialAssigments and assignmentRules
     * are handled is whether they are stuffed in the initialAssigment
     * or assigmentRules maps.
     *
     * This figures out what they refer to, and stuffs the AST in the
     * appropriate map.
     */
    void processElement(LLVMSymbolForest &currentSymbols,
            const libsbml::SBase *element, const libsbml::ASTNode *math);

    /**
     * specialized logic to write both amounts and concentrations here.
     */
    void processSpecies(LLVMSymbolForest &currentSymbols,
            const libsbml::Species *element, const libsbml::ASTNode *math);

    /**
     * get the MathML element for a SpeciesReference if it is set, otherwise,
     * create a ASTNode from its stoichiometry field.
     */
    const libsbml::ASTNode *getSpeciesReferenceStoichMath(const libsbml::SpeciesReference *reference);

    LLVMSymbolForest initialValues;

    LLVMSymbolForest assigmentRules;

    /**
     * all ASTNodes we create are stored here.
     */
    LLVMASTNodeFactory nodes;

    /**
     * a species can appear more than once in a reaction, there can be
     * several stoichiometries for a species both as a product and as
     * a reactant, for example, we could have
     *
     * A + 2A + B -> A + C
     *
     * In this case, we calculate the stochiometry for each species as
     * S(A) = -1 -2 + 1 = -1
     * S(B) = -1 + 0 = -1
     * S(C) = 0 + 1 = 1
     *
     * Stochiometries can however change, so we have to delay evaluation
     * of them -- if we have a named species reference, we add an ASTNode
     * with a reference to it: this way, when it get evaluated, we can look
     * up to see if we have any assigment rules or intitial assigments
     * overriding the original value.
     */

    /**
     * each species has a list of stoichiometry nodes, this way it can appear
     * more than once.
     */
    typedef std::list<const libsbml::ASTNode*> ASTNodeList;

    /**
     * we reference the reactants and products by the species index
     */
    typedef std::map<int, ASTNodeList> IntASTNodeListMap;

    /**
     * hold the symbols that belong to a reactions.
     */
    struct ReactionSymbols {
        IntASTNodeListMap reactants;
        IntASTNodeListMap products;
    };

    /**
     * these are indexed by reaction index.
     *
     * the stoichiometry matrix is a N species x N reaction matrix.
     */
    std::vector<ReactionSymbols> reactions;

private:
    libsbml::Model const
    *model;
    const LLVMModelDataSymbols &symbols;
};

} /* namespace rr */
#endif /* LLVMLLVMModelSymbolsH */
