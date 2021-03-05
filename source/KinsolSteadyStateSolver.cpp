//
// Created by Ciaran on 26/02/2021.
//

#include "KinsolSteadyStateSolver.h"
#include <cassert>
#include "KinsolErrHandler.h"
#include "rrConfig.h"


namespace rr {


    KinsolSteadyStateSolver::KinsolSteadyStateSolver(ExecutableModel *executableModel)
            : SteadyStateSolver(), mModel(executableModel) {
        KinsolSteadyStateSolver::resetSettings();
    }

    void KinsolSteadyStateSolver::syncWithModel(ExecutableModel *m) {
        freeKinsol();
        mModel = m;
        if (m) {
            createKinsol();
        }
        updateKinsol();
    }

    void KinsolSteadyStateSolver::createKinsol() {
        if (!mModel) {
            return;
        }

        assert(mStateVector == nullptr && mKinsol_Memory == nullptr &&
               "calling createKinsol, but kinsol objects already exist");

        // when argument is null, returns size of state vector (see rrExecutableModel::getStateVector)
        int stateVectorSize = mModel->getStateVector(nullptr);

        // create our N_Vector
        mStateVector = N_VNew_Serial(stateVectorSize);
        assert(mStateVector && "Sundials failed to create N_Vector for state variables");

        fscale = N_VNew_Serial(stateVectorSize);
        assert(fscale && "Sundials failed to create N_Vector for fscale");
        N_VConst(getValueAsDouble("fScaleDefault"), fscale);

        uscale = N_VNew_Serial(stateVectorSize);
        assert(uscale && "Sundials failed to create N_Vector for fscale");
        N_VConst(getValueAsDouble("uScaleDefault"), uscale);

        // initialise to model values
        mModel->getStateVector(mStateVector->ops->nvgetarraypointer(mStateVector));

        int err;

        // allocate the main kinsol memory block
        mKinsol_Memory = KINCreate();

        assert(mKinsol_Memory && "Could not create kinsol memory block, Kinsol failed");

        // set our own error handler. This should be the first thing called after creating kinsol memory block
        // This is the only function where we need to collect the error code and decode it, since
        // the purpose of using this function is to enable automatic error handling.
        if ((err = KINSetErrHandlerFn(
                mKinsol_Memory,
                reinterpret_cast<KINErrHandlerFn>(kinsolErrHandler),
                this)
            ) != KIN_SUCCESS) {
            decodeKinsolError(err);
        }

        // Set the kinsol "user data".
        // Kinsol uses callback functions for computing the rate of change.
        // Kinsol passes a void* memory buffer to `FixedPointIteration::kinsolDyDtFcn` where we
        // cast back to "FixedPointIteration".
        KINSetUserData(mKinsol_Memory, (void *) this);

        // set some optional features of kinsol

        KINSetFuncNormTol(mKinsol_Memory, getValueAsDouble("fnormtol"));
        KINSetScaledStepTol(mKinsol_Memory, getValueAsDouble("scsteptol"));
        KINSetPrintLevel(mKinsol_Memory, getValueAsInt("kinLogLevel"));
    }

    void KinsolSteadyStateSolver::freeKinsol() {

        if (mKinsol_Memory) {
            KINFree(&mKinsol_Memory);
        }

        if (mStateVector) {
            N_VDestroy_Serial(mStateVector);
        }
        if (fscale) {
            N_VDestroy_Serial(fscale);
        }
        if (uscale) {
            N_VDestroy_Serial(uscale);
        }

        mKinsol_Memory = nullptr;
        mStateVector = nullptr;
        fscale = nullptr;
        uscale = nullptr;
    }

    void KinsolSteadyStateSolver::resetSettings() {
        Solver::resetSettings();

        // Set default integrator settings.
        // todo delete step if not needed
        std::string desc = "Step size used in calculation of steady state";
        addSetting("Step", 1, "Step", desc, desc);

        // fnormtol
        desc = "specifies the scalar used as a stopping tolerance on the scaled"
               " maximum norm of the system function F(u). Defaults to 'unit roundoff ^ 1/3'";
        addSetting("fnormtol", 0, "fnormtol", desc, desc);

        // scsteptol
        desc = "stopping tolerance on the minimum scaled step length. Defaults to 'unit roundoff ^ 2/3";
        addSetting("scsteptol", 0, "scsteptol", desc, desc);

        // kinsol logging level
        desc = "Integer between 0 and 3 inclusive. Higher the level, the more detail "
               "kinsol outputs during computation. Default 0.";
        addSetting("kinLogLevel", 0, "kinLogLevel", desc, desc);

        //fscale and uscale
        desc = "The default value used for the fscale parameter. This is the diagonal of a matrix used for scaling matrices "
               "internally in kinsol";
        addSetting("fScaleDefault", 0.1,
                   "fScaleDefault", desc, desc);
        desc = "The default value used for the uscale parameter. This is the diagonal of a matrix used for scaling matrices "
               "internally in kinsol";
        addSetting("uScaleDefault", 0.1,
                   "uScaleDefault", desc, desc);

    }

    void KinsolSteadyStateSolver::getSolverStats() {
        // todo fixme. Get main solver working first, then come back and polish up
//        KINGetWorkSpace(mKinsol_Memory, &);
//        KINGetNumFuncEvals(mKinsol_Memory, &);
//        KINGetNumNolinSolvIters(mKinsol_Memory, &);
//        KINGetNumBetaCondFails(mKinsol_Memory, &);
//        KINGetNumBacktrackOps(mKinsol_Memory, &);
//        KINGetFuncNorm(mKinsol_Memory, &);
//        KINGetStepLength(mKinsol_Memory, &);
//        KINGetLinWorkSpace(mKinsol_Memory, &);
//        KINGetNumJacEvals(mKinsol_Memory, &);
//        KINGetNumBacktrackOps(mKinsol_Memory, &backtrackOps);
//        KINGetNumBetaCondFails(mKinsol_Memory, &betaCondFails);
//        KINGetNumFuncEvals(mKinsol_Memory, &funcEvals);
//        KINGetNumJacEvals(mKinsol_Memory, &jacEvals);
//        KINGetNumJtimesEvals(mKinsol_Memory, &jtimesEvals);
//        KINGetNumLinConvFails(mKinsol_Memory, &linConvFails);
//        KINGetNumLinFuncEvals(mKinsol_Memory, &linFuncEvals);
//        KINGetNumLinIters(mKinsol_Memory, &linIters);
//        KINGetNumNonlinSolvIters(mKinsol_Memory, &nonlinSolvIters);
//        KINGetNumPrecEvals(mKinsol_Memory, &precEvals);
//        KINGetNumPrecSolves(mKinsol_Memory, &precSolves);
//        KINGetLastLinFlag(mKinsol_Memory, )
//        KINGetLinReturnFlagName(mmKinsol_Memory)
    }

    void KinsolSteadyStateSolver::setFScale(double value) {
        N_VConst(value, fscale);
    }

    void KinsolSteadyStateSolver::setFScale(const std::vector<double>& value) {
        int stateSize = mStateVector->ops->nvgetlength(mStateVector);
        if (value.size() != stateSize){
            std::ostringstream err;
            err << __FILE__ <<":"<<__LINE__<<":"<<__FUNC__
                <<": size of vector to set the fscale variable does not "
                  "equal the number of states in the model (" << stateSize <<"!="<<value.size()<<")"<<std::endl;
            throw std::runtime_error(err.str());
        }
        double* dptr = mStateVector->ops->nvgetarraypointer(fscale);
        *dptr = *value.data();
    }

    void KinsolSteadyStateSolver::setUScale(double value) {
        N_VConst(value, uscale);
    }

    void KinsolSteadyStateSolver::setUScale(std::vector<double> value) {
        int stateSize = mStateVector->ops->nvgetlength(mStateVector);
        if (value.size() != stateSize){
            std::ostringstream err;
            err << __FILE__ <<":"<<__LINE__<<":"<<__FUNC__
                <<": size of vector to set the uscale variable does not "
                  "equal the number of states in the model (" << stateSize <<"!="<<value.size()<<")"<<std::endl;
            throw std::runtime_error(err.str());
        }
        double* dptr = mStateVector->ops->nvgetarraypointer(uscale);
        *dptr = *value.data();
    }


}