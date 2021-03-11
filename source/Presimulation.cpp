//
// Created by Ciaran on 11/03/2021.
//

#include "Presimulation.h"
#include "CVODEIntegrator.h"

namespace rr {

    Presimulation::Presimulation(ExecutableModel *model, SteadyStateSolver *solver)
        : model_(model), solver_(solver){

    }

    void Presimulation::simulate(const Dictionary* options) {
        CVODEIntegrator integrator(model_);

        // configure any relevant options
        if (options) {
            const std::vector<std::string> &settings = integrator.getSettings();
            for (auto &setting: settings) {
                if (options->hasKey(setting)) {
                    integrator.setValue(setting, options->getItem(setting));
                }
            }
        }
        integrator.integrate(integrator.getValueAsDouble("start"), integrator.getValueAsDouble("duration") );
        integrator.syncWithModel(model_);
    }
}