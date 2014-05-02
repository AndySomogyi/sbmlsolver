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
        stoichScale(1.0)
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

    model->getStoichiometryMatrix(&stoichRows, &stoichCols, &stoichData);

    engine.seed((unsigned long) std::time(0));

}

GillespieIntegrator::~GillespieIntegrator()
{
    delete[] reactionRates;
    delete[] reactionRatesBuffer;
    delete[] stateVector;
    delete[] stateVectorRate;
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

/*
 void GridSection::ssa(int i, int j, double tf, pfunc *a, int* nu, int r) {
 printf("B: %f, P: %f, R: %f, init: %i, tf: %f, dt: %f\n", B, P, R, z, tf, dt);
 double t = 0;
 double ddt = 0;
 while(t < tf) {
 if(t >= ddt) {
 fprintf(file, "%f, %i\n", t, z);
 fprintf(f2, "%f, %f, %i\n", B, t, z);
 ddt += dt;
 }
 double p1 = drand();
 double p2 = drand();
 double s = 0;
 for(int k = 0; k < r; k++) {
 s += ((*this).*(a[k]))(i,j);
 }
 double h = -log(p1) / s;
 t = t+h;
 double ctr = 0;
 for(int k = 0; k < r; k++) {
 double next = ((*this).*(a[k]))(i,j) / s;
 if((ctr < p2) && (p2 < (ctr + next))) {
 z += nu[k];
 break;
 }
 ctr += next;
 }
 }
 }
 */

/*
static double rmin = std::numeric_limits<double>::max();
static double rmax = std::numeric_limits<double>::min();
static double sum = 0;
static uint count = 0;

static void add_stats(double p)
{
    rmin = p < rmin ? p : rmin;
    rmax = p > rmax ? p : rmax;
    sum += p;
    count++;
    std::cout << "avg: " << sum / count << ", p: " << p << ", min: " << rmin << ", max: " << rmax << std::endl;
}
*/

double GillespieIntegrator::integrate(double t, double hstep)
{
    double tf = 0;
    bool singleStep;

    std::memset(reactionRatesBuffer, 0, nReactions * sizeof(double));

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

    model->setTime(t);
    model->getStateVector(stateVector);

    while (t < tf)
    {
        double p1 = urand();
        double p2 = urand();

        assert(p1 >= 0 && p1 <= 1 && p2 >= 0 && p2 <= 1);

        double s = 0;

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
        double h = (-log(p1) / s) * timeScale;
        t = t + h;
        double ctr = 0;

        // find index of next reaction
        for (int k = 0; k < nReactions; k++)
        {
            double next = std::abs(reactionRates[k]) / s;
            if ((ctr < p2) && (p2 < (ctr + next)))
            {
                Log(Logger::LOG_DEBUG) << "time: " << t << ", reaction: "
                        << k;

                // if rate is negative, means reaction goes in reverse, so
                // multiply by sign
                double sign = (reactionRates[k] > 0) - (reactionRates[k] < 0);

                for (int i = floatingSpeciesStart; i < stateVectorSize; ++i)
                {
                    stateVector[i] = stateVector[i]
                            + getStoich(i - floatingSpeciesStart, k)
                            * stoichScale
                            * sign;
                }

                model->setStateVector(stateVector);
                break;
            }
            ctr += next;
        }

        // break out of loop and return if we are doing variable step
        if (singleStep)
        {
            break;
        }
    }

    return t;
}

void GillespieIntegrator::restart(double t0)
{
    engine.seed((unsigned long) std::time(0));
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
    return (double) engine() / (double) engine.max();
}

} /* namespace rr */
