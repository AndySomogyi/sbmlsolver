/*
 * TestEvalInitialConditions.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */

#include "TestEvalInitialConditions.h"
#include "rrLogger.h"

namespace rr
{

TestEvalInitialConditions::TestEvalInitialConditions(const std::string& version, int caseNumber)
    : TestBase(version, caseNumber)
{
}

TestEvalInitialConditions::~TestEvalInitialConditions()
{
}
    
bool TestEvalInitialConditions::test()
{
    Log(Logger::PRIO_INFORMATION) << "Evaluating Initial Conditions for " << fileName << endl;
    
    model->evalInitialConditions();
    
    Log(Logger::PRIO_INFORMATION) << model << endl;
    
    return true;
}

} /* namespace rr */
