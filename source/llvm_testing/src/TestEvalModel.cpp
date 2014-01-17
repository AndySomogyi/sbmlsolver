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
    Log(Logger::LOG_INFORMATION) << "Evaluating Initial Conditions for " << fileName << endl;

    model->evalInitialConditions();

    Log(Logger::LOG_INFORMATION) << model << endl;

    Log(Logger::LOG_INFORMATION) << "Evaluating Reaction Rates for " << fileName << endl;

    model->evalReactionRates();

    Log(Logger::LOG_INFORMATION) << model << endl;

    Log(Logger::LOG_INFORMATION) << "Evaluating Model for " << fileName << endl;

    model->getStateVectorRate(0, 0, 0);

    Log(Logger::LOG_INFORMATION) << model << endl;

    double vol = 23;

    model->setCompartmentInitVolumes(1, 0, &vol);

    Log(Logger::LOG_INFORMATION) << "After set init vol:" << endl;
    Log(Logger::LOG_INFORMATION) << model << endl;

    double newvol = 0;

    model->getCompartmentInitVolumes(1, 0, &newvol);

    Log(Logger::LOG_INFORMATION) << "new vol:" << newvol << endl;


    vol = -1;

    model->setCompartmentInitVolumes(1, 0, &vol);

    Log(Logger::LOG_INFORMATION) << "After set init vol:" << endl;
    Log(Logger::LOG_INFORMATION) << model << endl;

    newvol = 0;

    model->getCompartmentInitVolumes(1, 0, &newvol);

    Log(Logger::LOG_INFORMATION) << "new vol:" << newvol << endl;

    return true;
}

} /* namespace rr */
