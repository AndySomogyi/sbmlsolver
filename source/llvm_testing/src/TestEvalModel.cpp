/*
 * TestEvalModel.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "TestEvalModel.h"
#include "rrLogger.h"

namespace rr
{

using namespace std;

TestEvalModel::TestEvalModel(const std::string& compiler,
        const std::string& version, int caseNumber)
    : TestBase(compiler, version, caseNumber)
{
}

TestEvalModel::~TestEvalModel()
{
    // TODO Auto-generated destructor stub
}

bool TestEvalModel::test()
{
    Log(Logger::PRIO_INFORMATION) << "Evaluating Initial Conditions for " << fileName << endl;

    model->evalInitialConditions();

    Log(Logger::PRIO_INFORMATION) << model << endl;

    Log(Logger::PRIO_INFORMATION) << "Evaluating Reaction Rates for " << fileName << endl;

    model->evalReactionRates();

    Log(Logger::PRIO_INFORMATION) << model << endl;

    Log(Logger::PRIO_INFORMATION) << "Evaluating Model for " << fileName << endl;

    model->evalModel(0, 0, 0);

    Log(Logger::PRIO_INFORMATION) << model << endl;

    return true;
}

} /* namespace rr */
