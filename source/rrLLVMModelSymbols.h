/*
 * rrLLVMModelSymbols.h
 *
 *  Created on: Jul 16, 2013
 *      Author: andy
 */

#ifndef LLVMLLVMModelSymbolsH
#define LLVMLLVMModelSymbolsH

#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include <sbml/Model.h>
#include <sbml/SBMLVisitor.h>

namespace rr
{
using libsbml::Model;
using libsbml::Compartment;
using libsbml::SBMLVisitor;
using libsbml::Species;
using libsbml::Parameter;

class LLVMModelSymbols: private SBMLVisitor
{
    using SBMLVisitor::visit;

public:
    LLVMModelSymbols(const LLVMModelGeneratorContext &mgc);
    ~LLVMModelSymbols();


protected:

    enum SpeciesReferenceType {
        Reactant, Product
    };

    virtual bool visit(const libsbml::Compartment &x);
    virtual bool visit(const libsbml::Species &x);
    virtual bool visit(const libsbml::Parameter &x);
    virtual bool visit(const libsbml::AssignmentRule  &x);

    /**
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
    virtual bool visit(const libsbml::Reaction  &x);

    /**
     * tell the acceptor to process all rules, even the ones
     * we don't handle so the iteration continues over all rules.
     */
    virtual bool visit(const libsbml::Rule &x);

    void processElement(const libsbml::SBase *element, const ASTNode *math);

    /**
     * specialized logic to write both amounts and concentrations here.
     */
    void processSpecies(const libsbml::Species *element, const ASTNode *math);

    /**
     * determine if this is a reactant or a product (from its parents),
     * then call full processSpeciesReference to process it.
     */
    void processSpeciesReference(const libsbml::SpeciesReference *reference,
            const ASTNode *math);

    /**
     * If the reference type is set to product, then the stoich tree is left
     * alone (assumed positive), if its set to reactant, then the stoich tree
     * is multiplied by -1.
     *
     * If stoch is 0, then it is determined from the SpeciesReference stochiometry
     * or stochiometryMath.
     */
    void processSpeciesReference(const libsbml::SpeciesReference* reference,
            const libsbml::Reaction* reaction, SpeciesReferenceType type,
            const ASTNode* stoich);


    /**
     * create an ASTNode for the species id / reaction id pair.
     *
     * This assembles the mess of items stored in the reactions array.
     */
    const ASTNode *createStoichiometryNode(int row, int col);


    LLVMSymbolForest symbolForest;
    LLVMASTNodeFactory nodes;



    /**
     * what follows is a rather ugly data structure...
     *
     * there are many many ways of specifiying stoichiometry, this
     * data stucture is designed to accomodate all of them.
     *
     * Idea is all participants in this reaction have an ASTNode, this
     * struct allows indexing that ASTNode by either the species index,
     * so it can populate the stoichiometry matrix, but also by the
     * species reference is, so that assigment rules or initial assignments
     * can repace that node.
     *
     * Each species can appear more than once in a reaction, so
     * it needs to have a list of species references with it.
     */
    typedef std::list<int> IntList;
    typedef std::map<std::string, int> StringIntMap;
    typedef std::map<int, IntList> IntIntListMap;
    struct ReactionSymbols {
        std::vector<const libsbml::ASTNode*> nodes;
        IntIntListMap reactantIdx;         // indexed by floating species index
        StringIntMap reactantRefIds;        // indexed by species reference name
        IntIntListMap productIdx;          // indexed by floating species index
        StringIntMap productRefIds;         // indexed by species reference name
    };


    /**
     * these are indexed by reaction index.
     *
     * the stoichiometry matrix is a N species x N reaction matrix.
     */
    std::vector<ReactionSymbols> reactions;

private:
    libsbml::Model *model;
    const LLVMModelDataSymbols &symbols;
};

} /* namespace rr */
#endif /* LLVMLLVMModelSymbolsH */
