/*
 * TestEvalReactionRates.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "TestEvalReactionRates.h"
#include "rrLogger.h"

namespace rr
{

using namespace std;

TestEvalReactionRates::TestEvalReactionRates(const std::string& compiler,
        const std::string& version, int caseNumber)
: TestBase(compiler, version, caseNumber)
{
}

TestEvalReactionRates::~TestEvalReactionRates()
{
}

bool TestEvalReactionRates::test()
{
    Log(Logger::INFORMATION) << "Evaluating Initial Conditions for " << fileName << endl;

    model->evalInitialConditions();

    Log(Logger::INFORMATION) << model << endl;

    Log(Logger::INFORMATION) << "Evaluating Reaction Rates for " << fileName << endl;

    model->evalReactionRates();

    Log(Logger::INFORMATION) << model << endl;

    return true;
}



} /* namespace rr */
