//
// Created by Ciaran Welsh on 14/06/2021.
//

#include "FactoryWithRegistration.h"
#include "rrException.h"
#include "Solver.h"

#include <mutex>

namespace rr {
    static std::mutex mutex;

    FactoryWithRegistration::~FactoryWithRegistration() {
        for (auto it : registrars){
            delete it;
        }
    }

    Solver* FactoryWithRegistration::New(const std::string& name, ExecutableModel* m) const {
        for (auto it : registrars){
            if (it->getName() == name) {
                return it->construct(m);
            }
        }
        throw InvalidKeyException("No such integrator: " + name);
    }

    void FactoryWithRegistration::registerSolver(Registrar* i) {
        if (!i)
            throw CoreException("Registrar is null");
        registrars.push_back(i);
    }

//    FactoryWithRegistration& FactoryWithRegistration::getInstance() {
//        std::lock_guard<std::mutex> lockGuard(mutex);
//        static FactoryWithRegistration factory;
//        return factory;
//    }

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

