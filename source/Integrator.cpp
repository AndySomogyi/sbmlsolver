/*
 * Integrator.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: andy
 */

#include "Integrator.h"
#include "CVODEIntegrator.h"
#include "GillespieIntegrator.h"
#include "RK4Integrator.h"
#include "rrStringUtils.h"

namespace rr
{

/**
 * list of interator names, the index should correspond to the
 * Integrator::IntegratorId enum.
 */
static const char* integratorNames[] = {"cvode", "gillespie", "rk4"};

Integrator* IntegratorFactory::New(const Dictionary* dict, ExecutableModel* m)
{
    Integrator *result = 0;
    const SimulateOptions* opt = dynamic_cast<const SimulateOptions*>(dict);

    if(opt == NULL) {
        throw std::invalid_argument("Currently, options must be a SimulateOptions object");
    }

    if (opt->integrator == Integrator::GILLESPIE)
    {
        result = new GillespieIntegrator(m, opt);
    }
    else if(opt->integrator == Integrator::RK4)
    {
        result = new RK4Integrator(m, opt);
    }
    else
    {
        result = new CVODEIntegrator(m, opt);
    }

    return result;
}

std::vector<std::string> IntegratorFactory::getIntegratorNames()
{
    return std::vector<std::string>(&integratorNames[0],
            &integratorNames[Integrator::INTEGRATOR_END]);
}

std::vector<const Dictionary*> IntegratorFactory::getIntegratorOptions()
{
    const Dictionary* options[] = {
            CVODEIntegrator::getIntegratorOptions(),
            GillespieIntegrator::getIntegratorOptions(),
            RK4Integrator::getIntegratorOptions()
    };
    return std::vector<const Dictionary*>(&options[0], &options[3]);
}

const Dictionary* IntegratorFactory::getIntegratorOptions(
        const std::string& intName)
{
    Integrator::IntegratorId id = getIntegratorIdFromName(intName);

    switch(id) {
    case Integrator::CVODE:
        return CVODEIntegrator::getIntegratorOptions();
    case Integrator::GILLESPIE:
        return GillespieIntegrator::getIntegratorOptions();
    case Integrator::RK4:
        return RK4Integrator::getIntegratorOptions();
    default:
        throw std::invalid_argument("invalid integrator name");

    }
}

Integrator::IntegratorType IntegratorFactory::getIntegratorType(
        Integrator::IntegratorId i)
{
    if (i == Integrator::CVODE || i == Integrator::RK4) {
        return Integrator::DETERMINISTIC;
    } else {
        return Integrator::STOCHASTIC;
    }
}

Integrator::IntegratorId IntegratorFactory::getIntegratorIdFromName(const std::string& _name)
{
    std::string name = rr::toUpper(_name);
    

    for (unsigned i = 0; i < Integrator::INTEGRATOR_END; ++i) {
        std::string iname = rr::toUpper(getIntegratorNameFromId((Integrator::IntegratorId)i));

        if (iname == name) {
            return (Integrator::IntegratorId)i;
        }
    }

    throw std::invalid_argument("invalid integrator name");
}

std::string IntegratorFactory::getIntegratorNameFromId(Integrator::IntegratorId integrator)
{


    if (integrator >= 0 && integrator < Integrator::INTEGRATOR_END)
    {
        return integratorNames[integrator];
    }
    else
    {
        throw std::invalid_argument("Invalid integrator value");
    }
}

}
