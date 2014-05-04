/*
 * GillespieIntegrator.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: andy
 */

#include "GillespieIntegrator.h"
#include "rrUtils.h"
#include "rrLogger.h"

#include <cstring>
#include <assert.h>
#include <iostream>
#include <exception>
#include <ctime>
#include <limits>

using namespace std;

// min and max macros on windows interfer with max method of engine.
#undef max
#undef min

namespace rr
{

GillespieIntegrator::GillespieIntegrator(ExecutableModel* m,
        const SimulateOptions* o) :
        model(m),
        timeScale(1.0),
        stoichScale(1.0),
        stoichRows(0),
        stoichCols(0),
        stoichData(0)
{
    if (o)
    {
        this->options = *o;
    }

    nReactions = model->getNumReactions();
    reactionRates = new double[nReactions];
    reactionRatesBuffer = new double[nReactions];
    stateVectorSize = model->getStateVector(0);
    stateVector = new double[stateVectorSize];
    stateVectorRate = new double[stateVectorSize];

    floatingSpeciesStart = stateVectorSize - model->getNumIndFloatingSpecies();

    assert(floatingSpeciesStart >= 0);

    // get rows and columns
    model->getStoichiometryMatrix(&stoichRows, &stoichCols, 0);
    stoichData = new double[stoichRows * stoichCols];

    // fill stoichData
    model->getStoichiometryMatrix(&stoichRows, &stoichCols, &stoichData);


}

GillespieIntegrator::~GillespieIntegrator()
{
    delete[] reactionRates;
    delete[] reactionRatesBuffer;
    delete[] stateVector;
    delete[] stateVectorRate;
    delete[] stoichData;
}

void GillespieIntegrator::setSimulateOptions(const SimulateOptions* o)
{
    if (o)
    {
        options = *o;

        if(options.hasKey("stoichScale"))
        {
            Log(Logger::LOG_NOTICE) << "getting stoichScale";
            stoichScale = options.getValue("stoichScale").convert<double>();
            Log(Logger::LOG_NOTICE) << "new stoichScale: " << stoichScale;
        }


        if(options.hasKey("timeScale"))
        {
            Log(Logger::LOG_NOTICE) << "getting timeScale";
            timeScale = options.getValue("timeScale").convert<double>();
            Log(Logger::LOG_NOTICE) << "new timeScale: " << timeScale;
        }
    }
}



double GillespieIntegrator::integrate(double t, double hstep)
{
    double tf = 0;
    bool singleStep;

    if (options.integratorFlags && SimulateOptions::VARIABLE_STEP)
    {
        if (options.minimumTimeStep > 0.0)
        {
            tf = t + options.minimumTimeStep;
            singleStep = false;
        }
        else
        {
            tf = t + hstep;
            singleStep = true;
        }
    }
    else
    {
        tf = t + hstep;
        singleStep = false;
    }

    Log(Logger::LOG_DEBUG) << "ssa(" << t << ", " << tf << ")";

    // get the initial state vector
    model->setTime(t);
    model->getStateVector(stateVector);

    while (t < tf)
    {
        // random uniform numbers
        double r1 = urand();
        double r2 = urand();

        assert(r1 > 0 && r1 <= 1 && r2 >= 0 && r2 <= 1);

        // output
        // output(out, t, x, M);

        // get propensity
        // update_p(p, c, x);
        // sum_propencity = sum(p, N);

        // sum of propensities
        double s = 0;

        // next time
        double tau = 0;

        // get the 'propensity' -- reaction rates
        model->getReactionRates(nReactions, 0, reactionRates);

        // sum the propensity
        for (int k = 0; k < nReactions; k++)
        {
            Log(Logger::LOG_DEBUG) << "reac rate: " << k << ": "
                    << reactionRates[k];

            // if reaction rate is negative, that means reaction goes in reverse,
            // this is fine, we just have to reverse the stoichiometry,
            // but still need to sum the absolute value of the propensities
            // to get tau.
            s += std::abs(reactionRates[k]);
        }

        // sample tau
        if (s > 0)
        {
            tau = -log(r1) / s;
        }
        else
        {
            // no reaction occurs
            return std::numeric_limits<double>::infinity();
        }

        t = t + tau;

        // select reaction
        int reaction = -1;
        double sp = 0.0;

        r2 = r2 * s;
        for (int i = 0; i < nReactions; ++i)
        {
            sp += std::abs(reactionRates[i]);
            if (r2 < sp)
            {
                reaction = i;
                break;
            }
        }

        assert(reaction >= 0 && reaction < nReactions);

        // update chemical species
        // if rate is negative, means reaction goes in reverse, so
        // multiply by sign
        double sign = (reactionRates[reaction] > 0)
                - (reactionRates[reaction] < 0);

        for (int i = floatingSpeciesStart; i < stateVectorSize; ++i)
        {
            stateVector[i] = stateVector[i]
                    + getStoich(i - floatingSpeciesStart, reaction)
                            * stoichScale * sign;
        }

        // rates could be time dependent
        model->setTime(t);
        model->setStateVector(stateVector);

        if (singleStep)
        {
            return t;
        }
    }

    return t;
}

void GillespieIntegrator::restart(double t0)
{
#ifdef CPP_RANDOM
    engine.seed((unsigned long) std::time(0));
#else
    timeval tv = {0, 0};
    gettimeofday(&tv, 0);
    srand48((long)(tv.tv_sec * tv.tv_usec));
#endif
}

void GillespieIntegrator::setListener(IntegratorListenerPtr)
{
}

IntegratorListenerPtr GillespieIntegrator::getListener()
{
    return IntegratorListenerPtr();
}

double GillespieIntegrator::urand()
{
#ifdef CPP_RANDOM
    return (double) engine() / (double) engine.max();
#else
    return drand48();
#endif
}

} /* namespace rr */
