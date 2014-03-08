/*
 * Integrator.h
 *
 *  Created on: Sep 7, 2013
 *      Author: andy
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <stdexcept>
#include "rrLogger.h"
#include "rrOSSpecifics.h"

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


class IntegratorException: public std::runtime_error
{
public:
    explicit IntegratorException(const std::string& what) :
            std::runtime_error(what)
    {
        Log(rr::Logger::LOG_ERROR) << __FUNC__ << "what: " << what;
    }

    explicit IntegratorException(const std::string& what, const std::string &where) :
            std::runtime_error(what + "; In " + where)
    {
        Log(rr::Logger::LOG_ERROR) << __FUNC__ << "what: " << what << ", where: " << where;
    }
};

}

#endif /* INTEGRATOR_H_ */
