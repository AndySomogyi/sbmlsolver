/*
 * GillespieIntegrator.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: andy
 */

#include "GillespieIntegrator.h"

namespace rr
{

GillespieIntegrator::GillespieIntegrator(ExecutableModel* oModel, const SimulateOptions* options)
{
    // TODO Auto-generated constructor stub

}

GillespieIntegrator::~GillespieIntegrator()
{
    // TODO Auto-generated destructor stub
}

void GillespieIntegrator::setSimulateOptions(const SimulateOptions* options)
{
}

double GillespieIntegrator::integrate(double t0, double tf)
{
    return 0;
}

void GillespieIntegrator::restart(double t0)
{
}

void GillespieIntegrator::setListener(IntegratorListenerPtr)
{
}

IntegratorListenerPtr GillespieIntegrator::getListener()
{
    return IntegratorListenerPtr();
}

} /* namespace rr */
