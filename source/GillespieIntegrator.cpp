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


using namespace std;

// min and max macros on windows interfer with max method of engine.
#undef max
#undef min



namespace rr
{

GillespieIntegrator::GillespieIntegrator(ExecutableModel* m,
    const SimulateOptions* o) :
        model(m)
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

    engine.seed(std::time(0));

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

// TODO Major TODO: find out why we need fudge parameters!!!
const double sscale = 1;
const double tscale = 4;

double GillespieIntegrator::integrate(double t, double hstep)
{
    std::memset(reactionRatesBuffer, 0, nReactions * sizeof(double));

    double tf = t + hstep;

    Log(Logger::LOG_DEBUG) << "ssa(" << t << ", " << tf << ")";

    model->setTime(t);
    model->getStateVector(stateVector);

    while(t < tf) {
        double p1 = urand();
        double p2 = urand();

        assert(p1 >= 0 && p1 <= 1 && p2 >= 0 && p2 <= 1);

        double s = 0;

        // get the 'propensity' -- reaction rates
        model->getReactionRates(nReactions, 0, reactionRates);

        // sum the propensity
        for(int k = 0; k < nReactions; k++) {

            Log(Logger::LOG_DEBUG) << "reac rate: " << k << ": " << reactionRates[k];

            if (reactionRates[k] < 0) {
                string msg = "reaction rate " + rr::toString(k)
                + " is negative: " + rr::toString(reactionRates[k]);
                throw (domain_error(msg));
            }

            s += reactionRates[k];
        }
        double h = (-log(p1) / s) * tscale;
        t = t+h;
        double ctr = 0;

        // find index of next reaction
        for(int k = 0; k < nReactions; k++) {
            double next = reactionRates[k] / s;
            if((ctr < p2) && (p2 < (ctr + next))) {

                Log(Logger::LOG_DEBUG) << "time: " << t << ", reaction: " << k;

                for(int i = floatingSpeciesStart; i < stateVectorSize; ++i) {
                    stateVector[i] = stateVector[i] + getStoich(i - floatingSpeciesStart, k) * sscale;
                }

                model->setStateVector(stateVector);


                break;
            }
            ctr += next;
        }
    }



    return t;
}

void GillespieIntegrator::restart(double t0)
{
    engine.seed(std::time(0));
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
    return (double)engine() / (double)engine.max();
}

} /* namespace rr */
