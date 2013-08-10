/*
 * AssignmentRuleEvaluator.cpp
 *
 *  Created on: Jul 3, 2013
 *      Author: andy
 */

#include "AssignmentRuleEvaluator.h"
#include "rrOSSpecifics.h"
#include <iostream>


using namespace std;
using namespace libsbml;

namespace rr
{
AssignmentRuleEvaluator::AssignmentRuleEvaluator(
        LLVMSymbolForest& symbols) :
        symbols(symbols)
{
}

void AssignmentRuleEvaluator::evaluate(const Model& model)
{
    model.accept(*this);
}

bool AssignmentRuleEvaluator::visit(const AssignmentRule& x)
{
    cout << __FUNC__ << ", id: " << x.getId() << "\n";


    return true;
}


} /* namespace rr */



