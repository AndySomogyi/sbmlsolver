/*
 * GillespieIntegrator.h
 *
 *  Created on: Apr 23, 2014
 *      Author: andy
 */

#ifndef GILLESPIEINTEGRATOR_H_
#define GILLESPIEINTEGRATOR_H_

#include "Integrator.h"


namespace rr
{

class ExecutableModel;
class SimulateOptions;

class GillespieIntegrator: public Integrator
{
public:
    GillespieIntegrator(ExecutableModel* model, const SimulateOptions* options);

    virtual ~GillespieIntegrator();

    /**
     * Set the configuration parameters the integrator uses.
     */
    virtual void setSimulateOptions(const SimulateOptions* options);

    /**
     * integrates the model from t0 to tf.
     */
    virtual double integrate(double t0, double tf);

    /**
     * copies the state vector out of the model and into cvode vector,
     * re-initializes cvode.
     */
    virtual void restart(double t0);

    /**
     * the integrator can hold a single listener. If clients require multicast,
     * they can create a multi-cast listener.
     */
    virtual void setListener(IntegratorListenerPtr);

    /**
     * get the integrator listener
     */
    virtual IntegratorListenerPtr getListener();
};

} /* namespace rr */

#endif /* GILLESPIEINTEGRATOR_H_ */
