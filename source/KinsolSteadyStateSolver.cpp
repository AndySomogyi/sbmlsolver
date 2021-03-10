//
// Created by Ciaran on 26/02/2021.
//

#include "KinsolSteadyStateSolver.h"
#include <cassert>
#include "KinsolErrHandler.h"
#include "rrConfig.h"
#include "CVODEIntegrator.h"

namespace rr {

    KinsolSteadyStateSolver::KinsolSteadyStateSolver(ExecutableModel *executableModel)
            : SteadyStateSolver(executableModel) {
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

        // when argument is null, returns size of state std::vector (see rrExecutableModel::getStateVector)
        int stateVectorSize = mModel->getStateVector(nullptr);

        // create our N_Vector
        mStateVector = N_VNew_Serial(stateVectorSize);
        assert(mStateVector && "Sundials failed to create N_Vector for state variables");

        fscale = N_VNew_Serial(stateVectorSize);
        assert(fscale && "Sundials failed to create N_Vector for fscale");
        N_VConst(1, fscale); // no scaling. Implement if wanted.

        uscale = N_VNew_Serial(stateVectorSize);
        assert(uscale && "Sundials failed to create N_Vector for fscale");
        N_VConst(1, uscale); // no scaling. Implement if wanted.

        constraints = N_VNew_Serial(stateVectorSize);
        assert(constraints && "Sundials failed to create N_Vector for fscale");
        // constraints. If,
        //  0 -> No constraints
        //  1 -> >= 0
        // -1 -> <= 0
        //  2  -> >0
        // -2  -> <0
        N_VConst(2, constraints);

        if (getValueAsBool("allow_negative")) {
            KINSetConstraints(mKinsol_Memory, constraints);
        }


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

        updateKinsol();
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
        if (constraints) {
            N_VDestroy_Serial(constraints);
        }

        mKinsol_Memory = nullptr;
        mStateVector = nullptr;
        fscale = nullptr;
        uscale = nullptr;
        constraints = nullptr;
    }

    void KinsolSteadyStateSolver::resetSettings() {
        SteadyStateSolver::resetSettings();

        std::string desc = "Max. number of nonlinear iterations";
        addSetting("NumMaxIters", 200, "NumMaxIters", desc, desc);

        addSetting("allow_negative", false, "Allow negative values",
                   "Ensures non-negative results",
                   "(bool)Constrains the problem such that all values are non-negative at all times");


        desc = "When presimulation is set to true, use stiff solver for integration";
        addSetting("stiff", false, "stiff", desc, desc);

        desc = "Kinsol logger level. Default=0, no additional output. Max=3.";
        addSetting("PrintLevel", 0, "PrintLevel", desc, desc);

        desc = "Form of nu coefficient. One of eta_choice1, eta_choice2 or eta_constant";
        addSetting("EtaForm", "eta_choice1", "EtaForm", desc, desc);

        desc = "No initial matrix setup";
        addSetting("NoInitSetup", false, "NoInitSetup", desc, desc);

        desc = "No residual monitoring";
        addSetting("NoResMon", false, "NoResMon", desc, desc);

        desc = "Max. iterations without matrix setup";
        addSetting("MaxSetupCalls", 10, "MaxSetupCalls", desc, desc);

        desc = "Max. iterations without residual check";
        addSetting("MaxSubSetupCalls", 5, "MaxSubSetupCalls", desc, desc);

        desc = "Constant value of nu";
        addSetting("EtaConstValue", 0.1, "EtaConstValue", desc, desc);

        desc = "Value of gamma where 0 << gamma << 1.0. Use 0 to indidate default value of 0.9.";
        addSetting("EtaParamGamma", 0, "EtaParamGamma", desc, desc);

        desc = "Value of alpha where 1.0 < alpha < 2.0. Use 0 to indicate default value of 2.0. ";
        addSetting("EtaParamAlpha", 0, "EtaParamAlpha", desc, desc);

        desc = "Value of omega_min - lower bound residual monitoring";
        addSetting("ResMonMin", 0.00001, "ResMonParams", desc, desc);

        desc = "Value of omega_max - upper bound residual monitoring";
        addSetting("ResMonMax", 0.9, "ResMonParams", desc, desc);

        desc = "Constant value of omega";
        addSetting("ResMonConstValue", 0.9, "ResMonConstValue", desc, desc);

        desc = "Lower bound on epsilon";
        addSetting("NoMinEps", false, "NoMinEps", desc, desc);

        desc = "Max. scaled length of Newton step. If 0 use default value which is 1000*||D_u*u_0||2.";
        addSetting("MaxNewtonStep", 0, "MaxNewtonStep", desc, desc);

        desc = "Max. number of beta-condition failures";
        addSetting("MaxBetaFails", 10, "MaxBetaFails", desc, desc);

        desc = "Function-norm stopping tolerance. If 0 use default of uround^1/3.";
        addSetting("FuncNormTol", 0, "FuncNormTol", desc, desc);

        desc = "Scaled-step stopping tolerance. If 0 use default of uround^2/3";
        addSetting("ScaledSteptol", 0, "ScaledSteptol", desc, desc);

        desc = "Anderson Acceleration subspace size. Default is 0.";
        addSetting("MAA", 0, "MAA", desc, desc);

        desc = "Anderson Acceleration damping parameter";
        addSetting("DampingAA", 1.0, "DampingAA", desc, desc);

        desc = "The function KINSetRelErrFunc speciffies the relative error in computing F(u), which "
               "is used in the difference quotient approximation to the Jacobian matrix. "
               "Set to 0 for default which equals U = unit roundoff.";
        addSetting("RelErrFunc", 0, "DampingAA", desc, desc);

    }

    void KinsolSteadyStateSolver::getSolverStatsFromKinsol() {
        KINGetNumFuncEvals(mKinsol_Memory, &numFuncEvals);
        KINGetNumNonlinSolvIters(mKinsol_Memory, &numNolinSolvIters);
        KINGetNumBetaCondFails(mKinsol_Memory, &numBetaCondFails);
        KINGetNumBacktrackOps(mKinsol_Memory, &numBacktrackOps);
        KINGetFuncNorm(mKinsol_Memory, &funcNorm);
        KINGetStepLength(mKinsol_Memory, &stepLength);
        KINGetNumJacEvals(mKinsol_Memory, &numJacEvals);
        KINGetNumJtimesEvals(mKinsol_Memory, &numJtimesEvals);
        KINGetNumLinConvFails(mKinsol_Memory, &numLinConvFails);
        KINGetNumLinFuncEvals(mKinsol_Memory, &numLinFuncEvals);
        KINGetNumLinIters(mKinsol_Memory, &numLinIters);
        KINGetNumNonlinSolvIters(mKinsol_Memory, &numNonlinSolvIters);
        KINGetNumPrecEvals(mKinsol_Memory, &numPrecEvals);
        KINGetNumPrecSolves(mKinsol_Memory, &numPrecSolves);

    }

    void KinsolSteadyStateSolver::setFScale(double value) {
        N_VConst(value, fscale);
    }

    void KinsolSteadyStateSolver::setFScale(const std::vector<double> &value) {
        int stateSize = mStateVector->ops->nvgetlength(mStateVector);
        if (value.size() != stateSize) {
            std::ostringstream err;
            err << __FILE__ << ":" << __LINE__ << ":" << __FUNC__
                << ": size of std::vector to set the fscale variable does not "
                   "equal the number of states in the model (" << stateSize << "!=" << value.size() << ")" << std::endl;
            throw std::runtime_error(err.str());
        }
        double *dptr = mStateVector->ops->nvgetarraypointer(fscale);
        *dptr = *value.data();
    }

    void KinsolSteadyStateSolver::setUScale(double value) {
        N_VConst(value, uscale);
    }

    void KinsolSteadyStateSolver::setUScale(std::vector<double> value) {
        int stateSize = mStateVector->ops->nvgetlength(mStateVector);
        if (value.size() != stateSize) {
            std::ostringstream err;
            err << __FILE__ << ":" << __LINE__ << ":" << __FUNC__
                << ": size of std::vector to set the uscale variable does not "
                   "equal the number of states in the model (" << stateSize << "!=" << value.size() << ")" << std::endl;
            throw std::runtime_error(err.str());
        }
        double *dptr = mStateVector->ops->nvgetarraypointer(uscale);
        *dptr = *value.data();
    }

    void *KinsolSteadyStateSolver::getKinsolMemory() const {
        return mKinsol_Memory;
    }

    /**
     * The settings configuration system in roadrunner make this particular
     * aspect of interacting with sundials libraries a little akward.
     * When a user updates a setting with setValue, they are only
     * updating the value in roadrunner. The change doesn't occur in sundials
     * until we call the corresponding sundials function with the new value.
     * We cannot use regular "setters" because they won't get called when a user
     * changes a value. Instead, here we update all kinsol options
     * at once, and this method is called before we call KIN_Solve.
     */
    void KinsolSteadyStateSolver::updateKinsol() {
        KINSetNumMaxIters(mKinsol_Memory, getValueAsInt("NumMaxIters"));

        KINSetPrintLevel(mKinsol_Memory, getValueAsInt("PrintLevel"));

        // throw if invalid option chosen.
        std::vector<std::string> validEtaForms({"eta_choice1", "eta_choice2", "eta_constant"});
        const std::string &etaChoice = getValueAsString("EtaForm");
        if (std::find(validEtaForms.begin(), validEtaForms.end(), etaChoice) == validEtaForms.end()) {
            std::ostringstream err;
            err << "\"" << etaChoice << "\". Valid options are ";
            for (auto &x: validEtaForms) {
                err << "\"" << x << "\", ";
            }
            throw InvalidKeyException(err.str());
        }
        if (etaChoice == "eta_choice1") {
            KINSetEtaForm(mKinsol_Memory, KIN_ETACHOICE1);
        } else if (etaChoice == "eta_choice2") {
            KINSetEtaForm(mKinsol_Memory, KIN_ETACHOICE2);
        } else if (etaChoice == "eta_constant") {
            KINSetEtaForm(mKinsol_Memory, KIN_ETACONSTANT);
        }

        KINSetNoInitSetup(mKinsol_Memory, getValueAsBool("NoInitSetup"));
        KINSetNoResMon(mKinsol_Memory, getValueAsBool("NoResMon"));
        KINSetMaxSetupCalls(mKinsol_Memory, getValueAsInt("MaxSetupCalls"));
        KINSetMaxSubSetupCalls(mKinsol_Memory, getValueAsInt("MaxSubSetupCalls"));
        KINSetEtaConstValue(mKinsol_Memory, getValueAsDouble("EtaConstValue"));
        KINSetEtaParams(mKinsol_Memory, getValueAsDouble("EtaParamGamma"), getValueAsDouble("EtaParamAlpha"));
        KINSetResMonParams(mKinsol_Memory, getValueAsDouble("ResMonMin"), getValueAsDouble("ResMonMax"));
        KINSetResMonConstValue(mKinsol_Memory, getValueAsBool("ResMonConstValue"));
        KINSetNoMinEps(mKinsol_Memory, getValueAsBool("NoMinEps"));
        KINSetMaxNewtonStep(mKinsol_Memory, getValueAsInt("MaxNewtonStep"));
        KINSetMaxBetaFails(mKinsol_Memory, getValueAsInt("MaxBetaFails"));
        KINSetRelErrFunc(mKinsol_Memory, getValueAsDouble("RelErrFunc"));
        KINSetFuncNormTol(mKinsol_Memory, getValueAsDouble("FuncNormTol"));
        KINSetScaledStepTol(mKinsol_Memory, getValueAsDouble("ScaledSteptol"));
        KINSetMAA(mKinsol_Memory, getValueAsLong("MAA"));
        KINSetDampingAA(mKinsol_Memory, getValueAsDouble("DampingAA"));
        // constraints not implemented at the moment. Maybe later???
        // KINSetConstraints(mKinsol_Memory, getValueAsInt("Constraints"));
    }

    void KinsolSteadyStateSolver::doPresimulation() {
        bool allowPresimulation = getValueAsBool("allow_presimulation");
        if (!allowPresimulation)
            return;

        // do presimulation in side class so that we can test it.
        assert(mModel && "Model is null");

        Presimulation presimulation(
                mModel,
                getValueAsDouble("presimulation_time"),
                getValueAsInt("presimulation_maximum_steps"),
                getValueAsBool("stiff")
        );
        presimulation.simulate();
        // remember to update the model
        syncWithModel(mModel);
    }

    std::unordered_map<std::string, Variant> KinsolSteadyStateSolver::getSolverStats() {
        std::unordered_map<std::string, Variant> map;
        map["numFuncEvals"] = Variant(numFuncEvals);
        map["numNolinSolvIters"] = Variant(numNolinSolvIters);
        map["numBetaCondFails"] = Variant(numBetaCondFails);
        map["numBacktrackOps"] = Variant(numBacktrackOps);
        map["funcNorm"] = Variant(funcNorm);
        map["stepLength"] = Variant(stepLength);
        map["numJacEvals"] = Variant(numJacEvals);
        map["numJtimesEvals"] = Variant(numJtimesEvals);
        map["numLinConvFails"] = Variant(numLinConvFails);
        map["numLinFuncEvals"] = Variant(numLinFuncEvals);
        map["numLinIters"] = Variant(numLinIters);
        map["numNonlinSolvIters"] = Variant(numNonlinSolvIters);
        map["numPrecEvals"] = Variant(numPrecEvals);
        map["numPrecSolves"] = Variant(numPrecSolves);
        return map;
    }

    void KinsolSteadyStateSolver::printSolverStats() {
        std::cout << "Solver Stats: " << std::endl;
        for (auto &it: getSolverStats()) {
            std::cout << "\t" << it.first << " = " << it.second.toString() << std::endl;
        }
    }


    Presimulation::Presimulation(ExecutableModel *model, double presimulation_time, int presimulation_maximum_steps,
                                 bool stiff)
            : model_(model),
              presimulation_maximum_steps_(presimulation_maximum_steps),
              presimulation_time_(presimulation_time),
              stiff_(stiff) {}

    void Presimulation::simulate() {
        CVODEIntegrator integrator(model_);
        integrator.setValue("maximum_num_steps", presimulation_maximum_steps_);
        integrator.setValue("stiff", stiff_);
        integrator.integrate(0, presimulation_time_);

    }
}