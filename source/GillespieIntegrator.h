/*
 * GillespieIntegrator.h
 *
 *  Created on: Apr 23, 2014
 *      Author: andy
 */

#ifndef GILLESPIEINTEGRATOR_H_
#define GILLESPIEINTEGRATOR_H_

#include "Integrator.h"
#include "rrExecutableModel.h"

// bugs in gcc 44 c++ random generator
#if (__cplusplus >= 201103L) || defined(_MSC_VER)
    #include <random>
    #define cxx11_ns std
    #define RR_CXX_RANDOM 1
#else
    #include <stdlib.h>
    #include <sys/time.h>
#endif


namespace rr
{

class ExecutableModel;

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
     * integrates the model from t0 to t0 + hstep
     */
    virtual double integrate(double t0, double hstep);

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

private:
    ExecutableModel *model;
    SimulateOptions options;

#ifdef RR_CXX_RANDOM
    cxx11_ns::mt19937 engine;
#endif

    double timeScale;
    double stoichScale;



    int nReactions;

    // starting index of floating species
    int floatingSpeciesStart;

    double* reactionRates;
    double* reactionRatesBuffer;

    int stateVectorSize;
    double* stateVector;
    double* stateVectorRate;

    // m rows x n cols
    // offset = row*NUMCOLS + column

    int stoichRows;
    int stoichCols;
    double* stoichData;

    double urand();

    inline double getStoich(uint species, uint reaction) {
        return stoichData[species * stoichCols + reaction];
    }


};

} /* namespace rr */

#endif /* GILLESPIEINTEGRATOR_H_ */
