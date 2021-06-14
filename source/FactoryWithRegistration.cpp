//
// Created by Ciaran Welsh on 14/06/2021.
//

#include "FactoryWithRegistration.h"
#include "rrException.h"
#include "Solver.h"

namespace rr {


    FactoryWithRegistration::~FactoryWithRegistration() {
        for (auto it : registrars){
            delete *it;
        }
    }

    Solver* FactoryWithRegistration::New(std::string name, ExecutableModel* m) const {
        for (auto it : registrars){
            if ((*it)->getName() == name) {
                return (*it)->construct(m);
            }
        }
        throw InvalidKeyException("No such integrator: " + name);
    }

    void FactoryWithRegistration::registerIntegrator(Registrar* i) {
        if (!i)
            throw CoreException("Registrar is null");
        registrars.push_back(i);
    }

    FactoryWithRegistration& FactoryWithRegistration::getInstance() {
        // FIXME: not thread safe -- JKM, July 24, 2015.
        static FactoryWithRegistration factory;
        return factory;
    }

    std::size_t FactoryWithRegistration::size() const {
        return registrars.size();
    }

    std::string FactoryWithRegistration::name(std::size_t n) const {
        return registrars.at(n)->getName();
    }

    std::string FactoryWithRegistration::hint(std::size_t n) const {
        return registrars.at(n)->getHint();
    }

    std::string FactoryWithRegistration::description(std::size_t n) const {
        return registrars.at(n)->getDescription();
    }


}

