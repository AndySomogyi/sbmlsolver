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
    rrLog(Logger::LOG_INFORMATION) << "Evaluating Initial Conditions for " << fileName << std::endl;


    rrLog(Logger::LOG_INFORMATION) << "before set: " << model << std::endl;

    double val = 0;
    model->getCompartmentVolumes(1, 0, &val);

    rrLog(Logger::LOG_INFORMATION) << "initial volume: " << val << std::endl;

    val = 2;
    model->setCompartmentVolumes(1, 0, &val);

    rrLog(Logger::LOG_INFORMATION) << "after: " << model << std::endl;

    model->getCompartmentVolumes(1, 0, &val);

    rrLog(Logger::LOG_INFORMATION) << "new volume: " << val << std::endl;

    double amt = 2;

    model->setFloatingSpeciesAmounts(1, 0, &amt);

    rrLog(Logger::LOG_INFORMATION) << "after set amt: " << model << std::endl;

    model->setFloatingSpeciesConcentrations(1, 0, &amt);

    rrLog(Logger::LOG_INFORMATION) << "after set conc: " << model << std::endl;





    return true;
}

} /* namespace rr */
