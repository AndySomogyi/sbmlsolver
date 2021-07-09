//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "ForwardSensitivitySolver.h"
#include "ForwardSensitivitySolver.h"
#include "LLVMExecutableModel.h"
#include <nvector/nvector_serial.h>               /* access to serial N_Vector                    */
#include "sundials/sundials_dense.h"
#include "sundials/sundials_linearsolver.h"
#include "sundials/sundials_nonlinearsolver.h"
#include "sunnonlinsol/sunnonlinsol_newton.h"
#include "sunnonlinsol/sunnonlinsol_fixedpoint.h"
#include "sunmatrix/sunmatrix_dense.h"
#include "sunlinsol/sunlinsol_dense.h"
#include "cvodes/cvodes.h"

namespace rr {

#define FFSHandleError(errorCode) { \
    std::string _what = decodeSundialsError<ForwardSensitivitySolver>(this, err, true);\
    std::ostringstream _os;\
    _os << "Error: " << __FUNC__ <<": " << _what;\
    throw std::logic_error(_os.str());                                \
    }

    void ffsErrHandler(int error_code, const char *module, const char *function,
                       char *msg, void *eh_data) {
        ForwardSensitivitySolver *i = (ForwardSensitivitySolver *) eh_data;

        if (error_code < 0) {
            rrLog(Logger::LOG_ERROR) << "ForwardSensitivitySolver Error: " << decodeSundialsError(i, error_code, false)
                                     << ", Module: " << module << ", Function: " << function
                                     << ", Message: " << msg;

        } else if (error_code == CV_WARNING) {
            rrLog(Logger::LOG_WARNING) << "CVODE Warning: "
                                       << ", Module: " << module << ", Function: " << function
                                       << ", Message: " << msg;
        }
    }

    int FFSDyDtFcn(realtype time, N_Vector cv_y, N_Vector cv_ydot, void *userData) {
        double *y = NV_DATA_S(cv_y);
        double *ydot = NV_DATA_S(cv_ydot);
        ForwardSensitivitySolver *inst = (ForwardSensitivitySolver *) userData;

        assert(inst && "userData pointer is NULL in cvode dydt callback");

        ExecutableModel *model = inst->getModel();

        model->getStateVectorRate(time, y, ydot);

        if (!inst->stateVectorVariables &&
            inst->getStateVector() &&
            NV_LENGTH_S(inst->getStateVector()) == 1) {
            ydot[0] = 0.0;
        }

        rrLog(Logger::LOG_TRACE) << __FUNC__ << ", model: " << model;

        return CV_SUCCESS;
    }

    int FFSRootFcn(realtype time, N_Vector y_vector, realtype *gout, void *user_data) {
        ForwardSensitivitySolver *cvInstance = (ForwardSensitivitySolver *) user_data;

        assert(cvInstance && "user data pointer is NULL on CVODE root callback");

        ExecutableModel *model = cvInstance->getModel();

        double *y = NV_DATA_S(y_vector);

        model->getEventRoots(time, y, gout);

        return CV_SUCCESS;
    }

    void ForwardSensitivitySolver::constructorOperations() {

        if (mModel) {
            cvodeIntegrator = std::make_unique<CVODEIntegrator>(mModel);

            /**
             * Use the memory and state vector from cvode
             */

            Np = mModel->getNumGlobalParameters();

            // when no argument to whichParameter specified, so we assume all parameters
            if (whichParameters.empty()) {
                Ns = Np;
                whichParameters = getGlobalParameterNames();
            } else {
                Ns = (int) whichParameters.size();
            }
            // figure out indexes of parameters that we want sens for
            deducePlist();

            // populate p and pbar
            p = getModelParametersAsVector();
            pbar = getModelParametersAsVector(); // scaling factors, use value scaling. See sundials docs.

            if (settings.empty()) {
                resetSettings();
            }
        }
    }

    ForwardSensitivitySolver::ForwardSensitivitySolver(ExecutableModel *executableModel)
            : TimeSeriesSensitivitySolver(executableModel) {
        constructorOperations();
    }

    ForwardSensitivitySolver::ForwardSensitivitySolver(ExecutableModel *executableModel,
                                                       const std::vector<std::string> &whichParameters)
            : TimeSeriesSensitivitySolver(executableModel), whichParameters(whichParameters) {
        constructorOperations();
    }

    double ForwardSensitivitySolver::integrate(double tStart, double hstep) {
        return cvodeIntegrator->integrate(tStart, hstep);
    }

//    Matrix<double> ForwardSensitivitySolver::getSensitivities(){
//
//    };

    ForwardSensitivitySolver::~ForwardSensitivitySolver() {}

    std::string ForwardSensitivitySolver::getName() const {
        return "forward";
    }

    std::string ForwardSensitivitySolver::getDescription() const {
        return "Interface to the sundials cvodes forward sensitivities solver";
    }

    std::string ForwardSensitivitySolver::getHint() const {
        return "forward sensitivities solver";
    }

    Solver *ForwardSensitivitySolver::construct(ExecutableModel *executableModel) const {
        return new ForwardSensitivitySolver(executableModel);
    }

    std::string ForwardSensitivitySolver::toString() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() >\n";
        ss << "  name: " << getName() << "\n";
        ss << "  settings:\n";
        ss << getSettingsRepr();
        return ss.str();
    }

    std::string ForwardSensitivitySolver::toRepr() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() \"" << getName() << "\" " << settingsPyDictRepr() << " >\n";
        return ss.str();
    }

    void ForwardSensitivitySolver::resetSettings() {
        // reuse settings from cvodeIntegrator
        settings = cvodeIntegrator->getSettingsMap();
    }

    void ForwardSensitivitySolver::setModel(ExecutableModel *executableModel) {
        cvodeIntegrator->setModel(executableModel);
        mModel = executableModel;
    }

    void ForwardSensitivitySolver::loadConfigSettings() {

    }

    std::vector<std::string> ForwardSensitivitySolver::getGlobalParameterNames() {
        std::vector<std::string> out;
        for (int i = 0; i < Np; i++) {
            std::string name = mModel->getGlobalParameterId(i);
            out.push_back(name);
        }
        return out;
    }

    ParameterMap ForwardSensitivitySolver::getModelParametersAsMap() {
        ParameterMap m;
        int *idx = new int[Np];
        for (int i = 0; i < Np; i++) idx[i] = i;
        double *vals = new double[Np];
        mModel->getGlobalParameterValues(Np, idx, vals);
        // first get the parameter ids
        for (int i = 0; i < Np; i++) {
            std::string name = mModel->getGlobalParameterId(i);
            m.insert(std::pair<std::string, double>(name, vals[i]));
        }
        delete[] idx;
        delete[] vals;
        return m;
    }

    std::vector<double> ForwardSensitivitySolver::getModelParametersAsVector() {
        std::vector<double> vec(Np);
        auto *idx = new int[Np];
        auto *vals = new double[Np];
        for (int i = 0; i < Np; i++) {
            idx[i] = i;
        }
        mModel->getGlobalParameterValues(Np, idx, vals);
        for (int i = 0; i < Np; i++) {
            vec[i] = vals[i];
        }
        delete[] idx;
        delete[] vals;
        return vec;
    }

    void ForwardSensitivitySolver::deducePlist() {
        plist.clear();
        plist.resize(whichParameters.size());
        for (int i = 0; i < whichParameters.size(); i++) {
            const std::string &paramName = whichParameters.at(i);
            int idx = mModel->getGlobalParameterIndex(paramName);
            plist[i] = idx;
        }
    }

    N_Vector ForwardSensitivitySolver::getStateVector() {
        return mStateVector;
    }

    N_Vector *ForwardSensitivitySolver::getSensitivityVector() {
        return mSensitivityVector;
    }

    void ForwardSensitivitySolver::create() {
        if (!mModel) {
            return;
        }


        assert(mStateVector == nullptr && mCVODE_Memory == nullptr &&
               "calling create, but cvode objects already exist");

        // still need cvode state std::vector size if we have no vars, but have
        // events, needed so root finder works.
        int allocStateVectorSize = 0;
        // when argument is null, returns size of state std::vector (see rrExecutableModel::getStateVector)
        int realStateVectorSize = mModel->getStateVector(nullptr);


        // cvode return code
        int err;

        if (realStateVectorSize > 0) {
            cvodeIntegrator->stateVectorVariables = true;
            allocStateVectorSize = realStateVectorSize;
        } else if (mModel->getNumEvents() > 0) {
            allocStateVectorSize = 1;
            cvodeIntegrator->stateVectorVariables = false;
        } else {
            cvodeIntegrator->stateVectorVariables = false;
            return;
        }

        // allocate and init the cvode arrays
        mStateVector = N_VNew_Serial(allocStateVectorSize);
        cvodeIntegrator->variableStepPostEventState.resize(allocStateVectorSize);

        // set mStateVector to the values that are currently in the model
        auto states = new double[allocStateVectorSize];
        mModel->getStateVector(states);

        for (int i = 0; i < allocStateVectorSize; i++) {
            mStateVector->ops->nvgetarraypointer(mStateVector)[i] = states[i];
        }
        delete[] states;

        if ((bool) getValue("stiff")) {
            rrLog(Logger::LOG_INFORMATION) << "using stiff integrator";
            mCVODE_Memory = (void *) CVodeCreate(CV_BDF);
        } else {
            rrLog(Logger::LOG_INFORMATION) << "using non-stiff integrator";
            mCVODE_Memory = (void *) CVodeCreate(CV_ADAMS);
        }

        assert(mCVODE_Memory && "could not create Cvode, CVodeCreate failed");

        if ((err = CVodeSetErrHandlerFn(mCVODE_Memory, ffsErrHandler, this)) !=
            CV_SUCCESS) {
            FFSHandleError(err);
        }

        // use non default CVODE value here, default is too short
        // for some sbml tests.
        CVodeSetMaxNumSteps(mCVODE_Memory, cvodeIntegrator->mDefaultMaxNumSteps);

        double t0 = 0.0;

        if ((err = CVodeSetUserData(mCVODE_Memory, (void *) this)) != CV_SUCCESS) {
            FFSHandleError(err);
        }

        if ((err = CVodeInit(mCVODE_Memory, FFSDyDtFcn, t0, mStateVector)) != CV_SUCCESS) {
            FFSHandleError(err);
        }

        if (mModel->getNumEvents() > 0) {
            if ((err = CVodeRootInit(mCVODE_Memory, mModel->getNumEvents(), FFSRootFcn)) != CV_SUCCESS) {
                FFSHandleError(err);
            }
            rrLog(Logger::LOG_TRACE) << "CVRootInit executed.....";
        }

        /**
         * note the cvDirectDemo.c example from sundials is useful
         * to see the different cvode options in action
         */

        /* create fixed point nonlinear solver object */
        if ((bool) getValue("stiff")) {
            // as per the cvode docs (look closely at docs for CVodeCreate)
            // we use the default Newton iteration for stiff

            cvodeIntegrator->nonLinSolver = SUNNonlinSol_Newton(mStateVector);

            if (cvodeIntegrator->nonLinSolver == nullptr) {
                throw std::runtime_error("CVODEIntegrator::createCVODE: nonLinearSolver_ is nullptr\n");
            }

            if ((err = CVodeSetNonlinearSolver(mCVODE_Memory, cvodeIntegrator->nonLinSolver)) != CV_SUCCESS) {
                FFSHandleError(err);
            }

            // the newton method requires use of a linear solver, which we set up here.
            cvodeIntegrator->jac = SUNDenseMatrix(allocStateVectorSize, allocStateVectorSize);
            cvodeIntegrator->linSolver = SUNLinSol_Dense(mStateVector, cvodeIntegrator->jac);
            if (cvodeIntegrator->linSolver == nullptr) {
                throw std::runtime_error("CVODEIntegrator::createCVODE: call to SunLinSol_Dense returned nullptr. "
                                         "The size of the sundials matrix (created with SUNDenseMatrix) used for the jacobian "
                                         "(" + std::to_string(allocStateVectorSize) + "x" +
                                         std::to_string(allocStateVectorSize)
                                         + ") is inappropriate for this model\n");
            }
            if ((err = CVodeSetLinearSolver(mCVODE_Memory, cvodeIntegrator->linSolver, cvodeIntegrator->jac)) !=
                CV_SUCCESS) {
                FFSHandleError(err);
            };

            // Use a difference quotient Jacobian by not passing Jac to CVodeSetJacFn
            if ((err = CVodeSetJacFn(mCVODE_Memory, nullptr)) != CV_SUCCESS) {
                FFSHandleError(err);
            }
        } else {
            // and fixed point solver (which used to be called functional iteration)
            // for nonstiff problems
            cvodeIntegrator->nonLinSolver = SUNNonlinSol_FixedPoint(mStateVector, 0);
            if ((err = CVodeSetNonlinearSolver(mCVODE_Memory, cvodeIntegrator->nonLinSolver)) != CV_SUCCESS) {
                FFSHandleError(err);
            }
        }

        cvodeIntegrator->setCVODETolerances();
        mModel->resetEvents();
    }

    SensitivitySolverFactory &SensitivitySolverFactory::getInstance() {
        return RegistrationFactory::getInstance<SensitivitySolverFactory>(sensitivitySolverMutex);
    }

    void SensitivitySolverFactory::Register() {
        static bool flag = false;
        if (!flag) {
            std::lock_guard<std::mutex> mtx(sensitivityRegistrationMutex);
            SensitivitySolverFactory::getInstance().registerSolver(new ForwardSensitivitySolver);
            flag = true;
        }
    }


}
