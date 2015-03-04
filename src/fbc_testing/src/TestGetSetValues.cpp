/*
 * TestEvalInitialConditions.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "TestGetSetValues.h"
#include "rrLogger.h"



namespace rr
{

using namespace std;

TestGetSetValues::TestGetSetValues(const std::string& compiler,
        const std::string& version, int caseNumber)
    : TestBase(compiler, version, caseNumber)
{
}

TestGetSetValues::~TestGetSetValues()
{
}

bool TestGetSetValues::test()
{
    Log(Logger::LOG_INFORMATION) << "Evaluating Initial Conditions for " << fileName << endl;


    Log(Logger::LOG_INFORMATION) << "before set: " << model << endl;

    double val = 0;
    model->getCompartmentVolumes(1, 0, &val);

    Log(Logger::LOG_INFORMATION) << "initial volume: " << val << endl;

    val = 2;
    model->setCompartmentVolumes(1, 0, &val);

    Log(Logger::LOG_INFORMATION) << "after: " << model << endl;

    model->getCompartmentVolumes(1, 0, &val);

    Log(Logger::LOG_INFORMATION) << "new volume: " << val << endl;

    double amt = 2;

    model->setFloatingSpeciesAmounts(1, 0, &amt);

    Log(Logger::LOG_INFORMATION) << "after set amt: " << model << endl;

    model->setFloatingSpeciesConcentrations(1, 0, &amt);

    Log(Logger::LOG_INFORMATION) << "after set conc: " << model << endl;





    return true;
}

} /* namespace rr */
