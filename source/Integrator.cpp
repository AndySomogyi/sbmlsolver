/*
 * Integrator.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: andy
 */

#include "Integrator.h"
#include "CVODEIntegrator.h"
#include "GillespieIntegrator.h"

namespace rr
{

Integrator* Integrator::New(const SimulateOptions* opt, ExecutableModel* m)
{
    Integrator *result = 0;

    if (opt->integrator == SimulateOptions::GILLESPIE)
    {
        result = new GillespieIntegrator(m, opt);
    }
    else
    {
        result = new CVODEIntegrator(m, opt);
    }

    return result;
}

}
