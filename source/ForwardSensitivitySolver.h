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

    /**
     * @brief Time based sensivitity solver.
     * @details Uses CVODEIntegrator to integrate the ExecutableModel
     * and cvodes to compute sensitivity information at each time point.
     * Implements the TimeSeriesSensitivitySolver interface
     */
    class ForwardSensitivitySolver : public TimeSeriesSensitivitySolver {
    public:

        ForwardSensitivitySolver() = default;

        ~ForwardSensitivitySolver() override;

        ForwardSensitivitySolver(ExecutableModel *executableModel);

        ForwardSensitivitySolver(ExecutableModel *executableModel, const std::vector<std::string> &whichParameters);

        double integrate(double tStart, double hstep) override;

//        Matrix<double> getSensitivities() override;

        /**
         * @brief instantiate the code necessary to use cvodes
         */
        void create();

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
        ParameterMap getModelParametersAsMap();

        /**
         * @brief return a std::vector<double> of model parameters
         * in the order they appear in the model.
         */
        std::vector<double> getModelParametersAsVector();

        /**
         * @brief returns the indexes of parameters that user wants
         * sensitivities for, based off of whichParameters.
         */
        void deducePlist();

        /**
         * @brief retuns pointer to the state vector
         * used by sundials for solving ODE.
         */
        N_Vector getStateVector();

        /**
         * @brief retuns pointer to the state vector
         * used by sundials for storing sensitivity matrix.
         */
        N_Vector* getSensitivityVector();

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
        std::unique_ptr<CVODEIntegrator> cvodeIntegrator = nullptr;

        void constructorOperations();

        /**
         * @brief memory associate with cvodes. Should mirror
         * the CVODEIntegrator::mCVODE_Memory ptr
         */
        void *mCVODE_Memory;

        /**
         * @brief State vector. Should mirror
         * the CVODEIntegrator::mStateVector ptr
         */
        N_Vector mStateVector;

        /**
         * @brief Non-linear solver for sensitivity analysis
         */
        SUNNonlinearSolver NLSsens;

        /**
         * @brief place to store the sensitivities
         */
        N_Vector *mSensitivityVector;

        /**
         * @brief indicator for whether model has state vector variables or not
         * @details mirrors CVODEIntegrator
         */
        bool stateVectorVariables;

        friend int FFSDyDtFcn(realtype time, N_Vector cv_y, N_Vector cv_ydot, void *userData);

        friend int FFSRootFcn(realtype time, N_Vector y_vector, realtype *gout, void *user_data);

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
