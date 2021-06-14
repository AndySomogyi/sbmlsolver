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
#include "Registrar.h"
#include <typeinfo>
#include <iomanip>

// == CODE ====================================================


namespace rr {
    /********************************************************************************************
    * SteadyStateSolver FACTORY
    ********************************************************************************************/

    SteadyStateSolverFactory::~SteadyStateSolverFactory() {
        for (auto it : mRegisteredSteadyStateSolvers) {
            delete it;
        }
    }

    SteadyStateSolver *SteadyStateSolverFactory::New(const std::string &name, ExecutableModel *m) const {
        for (auto it: mRegisteredSteadyStateSolvers){
            if (it->getName() == name) {
                return dynamic_cast<SteadyStateSolver *>(it->construct(m));
            }
        }
        rrLog(Logger::LOG_ERROR) << "No such SteadyStateSolver '" << name << "'";
        throw InvalidKeyException("No such SteadyStateSolver: " + name);
    }

    void SteadyStateSolverFactory::registerSteadyStateSolver(Registrar *i) {
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

        addSetting("auto_moiety_analysis", Setting(true), "Automatic Moiety Analysis Computation",
                   "Toggle automatic inference of the need for moiety conservation analysis",
                   "(bool) When true, models that require moiety conservation analysis are "
                   "automatically analysed for conserved moieties before computing steady state. The flip "
                   "side is that this check will slow computation, which will have a measurable performance impact on "
                   "tasks that require repeated steady state computation *if* the model does not "
                   "require require moiety conservation analysis.");

        addSetting("allow_presimulation", Setting(true), "Allow Presimulation",
                   "Flag for starting steady state analysis with simulation (bool).",
                   "(bool) This flag does not affect the usage of NLEQ1/2 approximation routine when the default steady state solver fails");

        addSetting("presimulation_time", Setting(5.0), "Presimulation Time",
                   "End time for presimulation steady state analysis (double).",
                   "(double) presimulation_maximum_steps takes priority. Only used when allow_presimulation is True");

        addSetting("presimulation_times", Setting(std::vector<double>({0.1, 1, 10, 100, 1e3, 1e4})), "Presimulation Times",
                   "Vector of successive time points to try presimulation prior to solving for steady state (std::vector<double>).",
                   "(double) Similar to presimulation_time, but tries multiple time points before failing");

        addSetting("presimulation_maximum_steps", Setting((double)getValue("presimulation_time") * 100),
                   "Presimulation Maximum Steps",
                   "Maximum number of steps that can be taken for presimulation before steady state analysis (int).",
                   "(int) Takes priority over presimulation_time. Only used when allow_presimulation is True");

        addSetting("allow_approx", Setting(true), "Allow Approximiation",
                   "Flag for using steady state approximation routine when steady state solver fails (bool).",
                   "(bool) Approximation routine will run only when the default solver fails to fine a solution. This flag does not affect usage of approximation routine for pre-simulation");

        addSetting("approx_tolerance", Setting(1e-6), "Approximation Tolerance",
                   "Tolerance for steady state approximation routine (double).",
                   "(double) Absolute tolerance used by steady state approximation routine. Only used when steady state approximation routine is used");

        addSetting("approx_maximum_steps", Setting(10000), "Approximation Maximum Steps",
                   "Maximum number of steps that can be taken for steady state approximation routine (int).",
                   "(int) Takes priority over approx_time. Only used when steady state approximation routine is used");

        addSetting("approx_time", Setting(10000), "Approximation Time",
                   "End time for steady state approximation routine (double).",
                   "(double) approx_maximum_steps takes priority. Only used when steady state approximation routine is used");

    }
    void SteadyStateSolver::loadConfigSettings() {

        SteadyStateSolver::setValue("allow_presimulation", Setting(Config::getBool(Config::STEADYSTATE_PRESIMULATION)));
        SteadyStateSolver::setValue("presimulation_maximum_steps",
                             Setting(Config::getInt(Config::STEADYSTATE_PRESIMULATION_MAX_STEPS)));
        SteadyStateSolver::setValue("presimulation_time", Setting(Config::getDouble(Config::STEADYSTATE_PRESIMULATION_TIME)));
        SteadyStateSolver::setValue("allow_approx", Setting(Config::getBool(Config::STEADYSTATE_APPROX)));
        SteadyStateSolver::setValue("approx_tolerance", Setting(Config::getDouble(Config::STEADYSTATE_APPROX_TOL)));
        SteadyStateSolver::setValue("approx_maximum_steps", Setting(Config::getInt(Config::STEADYSTATE_APPROX_MAX_STEPS)));
        SteadyStateSolver::setValue("approx_time", Setting(Config::getDouble(Config::STEADYSTATE_APPROX_TIME)));
    }


}