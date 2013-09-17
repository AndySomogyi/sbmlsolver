/*
 * rrRoadRunnerOptions.cpp
 *
 *  Created on: Sep 9, 2013
 *      Author: andy
 */

#include "rrRoadRunnerOptions.h"

namespace rr
{

LoadSBMLOptions::LoadSBMLOptions()
{
    version = 0;
    size = sizeof(LoadSBMLOptions);
    integrator = CVODE;
    integratorOpt = 0;
    modelGeneratorOpt = ReadOnlyModel;
}

SimulateOptions::SimulateOptions()
{
    startTime = 0.0;
    endTime = 5.0;
    nDataPoints = 50;

    flags = 0;
}

} /* namespace rr */
