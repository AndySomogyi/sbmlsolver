/*
 * rrLLVMAssignmentRuleEvaluator.cpp
 *
 *  Created on: Jul 3, 2013
 *      Author: andy
 */

#include "rrLLVMAssignmentRuleEvaluator.h"
#include "rrOSSpecifics.h"
#include <iostream>


using namespace std;
using namespace libsbml;

namespace rr
{
LLVMAssignmentRuleEvaluator::LLVMAssignmentRuleEvaluator(
        LLVMSymbolForest& symbols) :
        symbols(symbols)
{
}

void LLVMAssignmentRuleEvaluator::evaluate(const Model& model)
{
    model.accept(*this);
}

bool LLVMAssignmentRuleEvaluator::visit(const AssignmentRule& x)
{
    cout << __FUNC__ << ", id: " << x.getId() << "\n";


    return true;
}


} /* namespace rr */



