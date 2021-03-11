// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file SteadyStateSolver.cpp
* @author ETS, WBC, JKM
* @date Apr 25, 2014
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner SteadyStateSolvers
**/

// == INCLUDES ================================================

#include "SteadyStateSolver.h"
#include "rrStringUtils.h"
#include "rrConfig.h"
#include "rrUtils.h"
#include <typeinfo>
#include <iomanip>

// == CODE ====================================================


namespace rr {
    /*------------------------------------------------------------------------------------------
        SteadyStateSolver
      ------------------------------------------------------------------------------------------*/

    void SteadyStateSolver::loadConfigSettings() {
        // empty
    }

    SteadyStateSolverRegistrar::~SteadyStateSolverRegistrar() {}

    /********************************************************************************************
    * SteadyStateSolver FACTORY
    ********************************************************************************************/

    SteadyStateSolverFactory::~SteadyStateSolverFactory() {
        for (SteadyStateSolverRegistrars::const_iterator it(mRegisteredSteadyStateSolvers.begin());
             it != mRegisteredSteadyStateSolvers.end(); ++it) {
            delete *it;
        }
    }

    SteadyStateSolver *SteadyStateSolverFactory::New(const std::string &name, ExecutableModel *m) const {
        for (SteadyStateSolverRegistrars::const_iterator it(mRegisteredSteadyStateSolvers.begin());
             it != mRegisteredSteadyStateSolvers.end(); ++it) {
            if ((*it)->getName() == name) {
                return (*it)->construct(m);
            }
        }
        rrLog(Logger::LOG_ERROR) << "No such SteadyStateSolver '" << name << "'";
        throw InvalidKeyException("No such SteadyStateSolver: " + name);
    }

    void SteadyStateSolverFactory::registerSteadyStateSolver(SteadyStateSolverRegistrar *i) {
        if (!i)
            throw CoreException("Registrar is null");
        mRegisteredSteadyStateSolvers.push_back(i);
    }

    SteadyStateSolverFactory &SteadyStateSolverFactory::getInstance() {
        // FIXME: not thread safe -- JKM, July 24, 2015.
        static SteadyStateSolverFactory factory;
        return factory;
    }

    std::size_t SteadyStateSolverFactory::getNumSteadyStateSolvers() const {
        return mRegisteredSteadyStateSolvers.size();
    }

    std::vector<std::string> SteadyStateSolverFactory::getListSteadyStateSolverNames() {
        std::vector<std::string> intgNames;
        int numIntgs = static_cast<int>(SteadyStateSolverFactory::getInstance().mRegisteredSteadyStateSolvers.size());
        for (int i = 0; i < numIntgs; ++i) {
            std::size_t n = static_cast<std::size_t>(i);
            intgNames.push_back(SteadyStateSolverFactory::getInstance().getSteadyStateSolverName(n));
        }
        return intgNames;
    }

    std::string SteadyStateSolverFactory::getSteadyStateSolverName(std::size_t n) const {
        return mRegisteredSteadyStateSolvers.at(n)->getName();
    }

    std::string SteadyStateSolverFactory::getSteadyStateSolverHint(std::size_t n) const {
        return mRegisteredSteadyStateSolvers.at(n)->getHint();
    }

    std::string SteadyStateSolverFactory::getSteadyStateSolverDescription(std::size_t n) const {
        return mRegisteredSteadyStateSolvers.at(n)->getDescription();
    }

    std::string SteadyStateSolver::toString() const {
        std::stringstream ss;
        ss << "< roadrunner.SteadyStateSolver() >\n";
        ss << "  name: " << getName() << "\n";
        ss << "  settings:\n";
        ss << getSettingsRepr();
        return ss.str();
    }

    std::string SteadyStateSolver::toRepr() const {
        std::stringstream ss;
        ss << "< roadrunner.SteadyStateSolver() \"" << getName() << "\" " << settingsPyDictRepr() << " >\n";
        return ss.str();
    }

    void SteadyStateSolver::resetSettings() {
        Solver::resetSettings();
        addSetting("allow_presimulation", true, "Allow Presimulation",
                   "Flag for starting steady state analysis with simulation (bool).",
                   "(bool) This flag does not affect the usage of NLEQ1/2 approximation routine when the default steaty state solver fails");

        addSetting("presimulation_time", 1, "Presimulation Time",
                   "End time for presimulation steady state analysis (double).",
                   "(double) presimulation_maximum_steps takes priority. Only used when allow_presimulation is True");

        addSetting("presimulation_maximum_steps", getValueAsDouble("presimulation_time")*100, "Presimulation Maximum Steps",
                   "Maximum number of steps that can be taken for presimulation before steady state analysis (int).",
                   "(int) Takes priority over presimulation_time. Only used when allow_presimulation is True");

        addSetting("allow_approx", false, "Allow Approximiation",
                   "Flag for using steady state approximation routine when steady state solver fails (bool).",
                   "(bool) Approximation routine will run only when the default solver fails to fine a solution. This flag does not affect usage of approximation routine for pre-simulation");

        addSetting("approx_tolerance", 1e-6, "Approximation Tolerance",
                   "Tolerance for steady state approximation routine (double).",
                   "(double) Absolute tolerance used by steady state approximation routine. Only used when steady state approximation routine is used");

        addSetting("approx_maximum_steps", 10000, "Approximation Maximum Steps",
                   "Maximum number of steps that can be taken for steady state approximation routine (int).",
                   "(int) Takes priority over approx_time. Only used when steady state approximation routine is used");

        addSetting("approx_time", 10000, "Approximation Time",
                   "End time for steady state approximation routine (double).",
                   "(double) approx_maximum_steps takes priority. Only used when steady state approximation routine is used");

    }

}