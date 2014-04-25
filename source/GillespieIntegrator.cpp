/*
 * GillespieIntegrator.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: andy
 */

#include "GillespieIntegrator.h"

#if __cplusplus >= 201103L || defined(_MSC_VER)
#include <random>
#else
#include <tr1/random>
#endif

namespace rr
{

GillespieIntegrator::GillespieIntegrator(ExecutableModel* m,
        const SimulateOptions* o)
: model(m)
{
    if (o)
    {
        this->options = *o;
    }
}

GillespieIntegrator::~GillespieIntegrator()
{
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

double GillespieIntegrator::integrate(double t, double tf)
{
    /*
    double ddt = 0;
    while(t < tf) {
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
    */
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
