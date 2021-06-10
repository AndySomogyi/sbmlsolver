//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "ForwardSensitivities.h"

namespace rr{

    std::string ForwardSensitivities::getName() const {
        return "forward";
    }

    std::string ForwardSensitivities::getDescription() const {
        return "Interface to the sundials cvodes forward sensitivities solver";
    }

    std::string ForwardSensitivities::getHint() const {
        return "forward sensitivities solver";
    }

}