/*
 * AssignmentRuleEvaluator.cpp
 *
 *  Created on: Jul 3, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "AssignmentRuleEvaluator.h"
#include "rrOSSpecifics.h"
#include <iostream>



using namespace libsbml;

namespace rrllvm
{
AssignmentRuleEvaluator::AssignmentRuleEvaluator(
        SymbolForest& symbols) :
        symbols(symbols)
{
}

void AssignmentRuleEvaluator::evaluate(const Model& model)
{
    model.accept(*this);
}

bool AssignmentRuleEvaluator::visit(const AssignmentRule& x)
{
    std::cout << __FUNC__ << ", id: " << x.getId() << "\n";


    return true;
}


} /* namespace rr */



