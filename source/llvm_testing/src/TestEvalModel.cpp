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
    rrLog(Logger::LOG_INFORMATION) << "Evaluating Initial Conditions for " << fileName << std::endl;


    rrLog(Logger::LOG_INFORMATION) << model << std::endl;

    rrLog(Logger::LOG_INFORMATION) << "Evaluating Reaction Rates for " << fileName << std::endl;


    rrLog(Logger::LOG_INFORMATION) << model << std::endl;

    rrLog(Logger::LOG_INFORMATION) << "Evaluating Model for " << fileName << std::endl;

    model->getStateVectorRate(0, 0, 0);

    rrLog(Logger::LOG_INFORMATION) << model << std::endl;

    double vol = 23;

    model->setCompartmentInitVolumes(1, 0, &vol);

    rrLog(Logger::LOG_INFORMATION) << "After set init vol:" << std::endl;
    rrLog(Logger::LOG_INFORMATION) << model << std::endl;

    double newvol = 0;

    model->getCompartmentInitVolumes(1, 0, &newvol);

    rrLog(Logger::LOG_INFORMATION) << "new vol:" << newvol << std::endl;


    vol = -1;

    model->setCompartmentInitVolumes(1, 0, &vol);

    rrLog(Logger::LOG_INFORMATION) << "After set init vol:" << std::endl;
    rrLog(Logger::LOG_INFORMATION) << model << std::endl;

    newvol = 0;

    model->getCompartmentInitVolumes(1, 0, &newvol);

    rrLog(Logger::LOG_INFORMATION) << "new vol:" << newvol << std::endl;

    return true;
}

} /* namespace rr */
