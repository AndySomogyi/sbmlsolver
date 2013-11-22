/*
 * Integrator.h
 *
 *  Created on: Sep 7, 2013
 *      Author: andy
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

namespace rr
{

class Integrator
{
public:

    virtual unsigned setTolerances(double relative, double absolute) = 0;

    /**
     * get the tolerences used by the integrator
     *
     *
     */
    virtual unsigned getTolerances(double *relative, double *absolute) = 0;
};

}

#endif /* INTEGRATOR_H_ */
