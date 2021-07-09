//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "ForwardSensitivitySolver.h"
#include "ForwardSensitivitySolver.h"
#include "LLVMExecutableModel.h"

namespace rr {


    void ForwardSensitivitySolver::constructorOperations() {

        if (mModel) {
            cvodeIntegrator = std::make_unique<CVODEIntegrator>(mModel);

            /**
             * Use the memory and state vector from cvode
             */
//            mCVODE_Memory = cvodeIntegrator.getCvodeMemory();
//            mStateVector = cvodeIntegrator.getStateVector();

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
        }
    }

    ForwardSensitivitySolver::ForwardSensitivitySolver(ExecutableModel *executableModel)
            : SensitivitySolver(executableModel) {
        constructorOperations();
    }

    ForwardSensitivitySolver::ForwardSensitivitySolver(ExecutableModel *executableModel,
                                                       const std::vector<std::string> &whichParameters)
            : SensitivitySolver(executableModel), whichParameters(whichParameters) {
        constructorOperations();
    }

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
        for (int i=0; i<Np; i++) idx[i] = i;
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
        for (int i=0; i< whichParameters.size(); i++){
            const std::string& paramName = whichParameters.at(i);
            int idx = mModel->getGlobalParameterIndex(paramName);
            plist[i] = idx;
        }
    }

    void ForwardSensitivitySolver::create() {
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
