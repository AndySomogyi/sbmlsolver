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
    modelGeneratorOpt = 0;
}

SimulateOptions::SimulateOptions()
{

}

} /* namespace rr */
