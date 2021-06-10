//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "AdjointSensitivities.h"

namespace rr {

    std::string AdjointSensitivities::getName() const {
        return "adjoint";
    }

    std::string AdjointSensitivities::getDescription() const {
        return "Interface to the sundials cvodes adjoint sensitivities solver";
    }

    std::string AdjointSensitivities::getHint() const {
        return "adjoint sensitivities solver";
    }

    SensitivityResult AdjointSensitivities::solve() {
        return SensitivityResult();
    }


}

