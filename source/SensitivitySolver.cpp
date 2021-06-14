//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "SensitivitySolver.h"
#include "ForwardSensitivitySolver.h"
#include "AdjointSensitivitySolver.h"
#include "LLVMExecutableModel.h"

namespace rr {
    
    SensitivitySolverRegistrar::~SensitivitySolverRegistrar() {}

    /********************************************************************************************
    * SensitivitySolver FACTORY
    ********************************************************************************************/

    SensitivitySolverFactory::~SensitivitySolverFactory() {
        for (SensitivitySolverRegistrars::const_iterator it(mRegisteredSensitivitySolvers.begin());
             it != mRegisteredSensitivitySolvers.end(); ++it) {
            delete *it;
        }
    }

    SensitivitySolver *SensitivitySolverFactory::New(const std::string &name, ExecutableModel *m) const {
        for (SensitivitySolverRegistrars::const_iterator it(mRegisteredSensitivitySolvers.begin());
             it != mRegisteredSensitivitySolvers.end(); ++it) {
            if ((*it)->getName() == name) {
                return (*it)->construct(m);
            }
        }
        rrLog(Logger::LOG_ERROR) << "No such SensitivitySolver '" << name << "'";
        throw InvalidKeyException("No such SensitivitySolver: " + name);
    }

    void SensitivitySolverFactory::registerSensitivitySolver(SensitivitySolverRegistrar *i) {
        if (!i)
            throw CoreException("Registrar is null");
        mRegisteredSensitivitySolvers.push_back(i);
    }

    SensitivitySolverFactory &SensitivitySolverFactory::getInstance() {
        // FIXME: not thread safe -- JKM, July 24, 2015.
        static SensitivitySolverFactory factory;
        return factory;
    }

    std::size_t SensitivitySolverFactory::getNumSensitivitySolvers() const {
        return mRegisteredSensitivitySolvers.size();
    }

    std::vector<std::string> SensitivitySolverFactory::getListSensitivitySolverNames() {
        std::vector<std::string> intgNames;
        int numIntgs = static_cast<int>(SensitivitySolverFactory::getInstance().mRegisteredSensitivitySolvers.size());
        for (int i = 0; i < numIntgs; ++i) {
            std::size_t n = static_cast<std::size_t>(i);
            intgNames.push_back(SensitivitySolverFactory::getInstance().getSensitivitySolverName(n));
        }
        return intgNames;
    }

    std::string SensitivitySolverFactory::getSensitivitySolverName(std::size_t n) const {
        return mRegisteredSensitivitySolvers.at(n)->getName();
    }

    std::string SensitivitySolverFactory::getSensitivitySolverHint(std::size_t n) const {
        return mRegisteredSensitivitySolvers.at(n)->getHint();
    }

    std::string SensitivitySolverFactory::getSensitivitySolverDescription(std::size_t n) const {
        return mRegisteredSensitivitySolvers.at(n)->getDescription();
    }

    std::string SensitivitySolver::toString() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() >\n";
        ss << "  name: " << getName() << "\n";
        ss << "  settings:\n";
        ss << getSettingsRepr();
        return ss.str();
    }

    std::string SensitivitySolver::toRepr() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() \"" << getName() << "\" " << settingsPyDictRepr() << " >\n";
        return ss.str();
    }

    void SensitivitySolver::resetSettings() {
        CVODEIntegrator::resetSettings(); // integrator settings affect sensitivity

    }

    void SensitivitySolver::loadConfigSettings() {}




}
