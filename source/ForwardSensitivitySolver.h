//
// Created by Ciaran Welsh on 09/06/2021.
//

#ifndef ROADRUNNER_SENSITIVITIES_H
#define ROADRUNNER_SENSITIVITIES_H

#include <iostream>
#include "CVODEIntegrator.h"
#include "SensitivitySolver.h"
#include <unordered_map>
#include <vector>
#include "LLVMExecutableModel.h"

namespace rr {

    using ParameterMap = std::unordered_map<std::string, double>;

    class CVODEIntegrator;

    class ForwardSensitivitySolver : public SensitivitySolver {
    public:

        ForwardSensitivitySolver() = default;

        ForwardSensitivitySolver(ExecutableModel *executableModel);

        ForwardSensitivitySolver(ExecutableModel *executableModel, const std::vector<std::string> &whichParameters);

        /**
        * @brief Get the name of this solver
        */
        std::string getName() const override;

        /**
        * @brief Get the description of this solver
        */
        std::string getDescription() const override;

        /**
        * @brief Get a (user-readable) hint for this solver
        */
        std::string getHint() const override;

        Solver *construct(ExecutableModel *executableModel) const override;

        /**
         * @brief resets all settings back to default values
         */
        void resetSettings() override;

        void setModel(ExecutableModel *executableModel) override;

        std::string toRepr() const override;

        void loadConfigSettings();

        std::string toString() const override;

        /**
         * @brief returns a vector of global parameter names
         * extracted from the model
         */
        std::vector<std::string> getGlobalParameterNames();

        /**
         * @brief get global parameters as an unordered map, strings as
         * keys and parameter values as values
         */
        ParameterMap getModelParametersAsMap() ;

        /**
         * @brief return a std::vector<double> of model parameters
         * in the order they appear in the model.
         */
        std::vector<double> getModelParametersAsVector() ;

        /**
         * @brief returns the indexes of parameters that user wants
         * sensitivities for, based off of whichParameters.
         */
        void deducePlist();

        /**
         * @brief parameters in the model as a member variable
         * which enables passing the underlying data pointer to
         * sundials for finite difference approx
         * @details This is the full vector of model global
         * parameters in the order indexed in the model.
         * @see plist
         */
        std::vector<double> p;

        /**
         * @brief scaling factors. Set to the values of
         * the parameters for sensitivity. This should
         * be identical to p as acceptible default.
         */
        std::vector<double> pbar;

        /**
         * @brief which parameters to get sensitivity for as int?
         * @details indexes parameters in p and pbar.
         */
        std::vector<int> plist;

        /**
         * @brief which parameters to get sensitivities for, as strings
         */
        std::vector<std::string> whichParameters;

        /**
         * @brief a map containing model parameter names to values
         */
         ParameterMap globalParameterMap;


        /**
         * @brief number of global parameters in the model
         */
        int Np = 0;

        /**
         * @brief number of parameters we want to find sensitivities for
         */
        int Ns = 0;

    private:

        /**
         * @brief An instance of CVODEIntegrator is needed
         * for the forward sensitivity solver
         * @details The ForwardSensitivitySolver is in essence a CVODEIntegrator and a
         * SensitivitySolver. However we avoid the diamond inheritance problem that
         * would arise from inheriting from both and the consequent need to use
         * virtual inheritance, which may come with hidden problems;
         */
        CVODEIntegrator cvodeIntegrator;

        void constructorOperations();
    };

    /**
     * To make the singleton SensitivitySolverFactory thread safe
     */
    static std::mutex sensitivitySolverMutex;
    static std::mutex sensitivityRegistrationMutex;


    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::SensitivitySolver.
     */
    class RR_DECLSPEC SensitivitySolverFactory : public RegistrationFactory {
    public:
        static SensitivitySolverFactory &getInstance();

        static void Register();
    };


}

#endif //ROADRUNNER_SENSITIVITIES_H
