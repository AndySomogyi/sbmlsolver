/*
 * AssignmentRuleEvaluator.h
 *
 *  Created on: Jul 3, 2013
 *      Author: andy
 */

#ifndef LLVMAssignmentRuleEvaluatorH
#define LLVMAssignmentRuleEvaluatorH

#include "SymbolForest.h"

#include <sbml/Model.h>
#include <sbml/SBMLVisitor.h>
#include <sbml/AlgebraicRule.h>
#include <sbml/AssignmentRule.h>

namespace rr
{

/**
 * Attaches to an existing symbol forest and applies
 * any asigment rule found in the model to the forest.
 */
class AssignmentRuleEvaluator : public libsbml::SBMLVisitor
{
    using libsbml::SBMLVisitor::visit;

public:
    AssignmentRuleEvaluator(LLVMSymbolForest &symbols);

    void evaluate(const libsbml::Model& model);

    virtual bool visit (const libsbml::AssignmentRule  &x);

protected:
    LLVMSymbolForest &symbols;
};

} /* namespace rr */
#endif /* LLVMAssignmentRuleEvaluatorH */
