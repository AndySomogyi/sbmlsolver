//
// Created by Ciaran on 05/08/2020.
//

#include "SBMLFactory.h"

namespace rrtest {
    std::string rrtest::Bistable::str() {
        return std::string();
    }


    std::string SBMLFactory(ModelType modelType) {
        if (modelType == BISTABLE) {
            return Bistable().str();
//            else if (modelType == SBML_NOT_ANNOTATED)
//                return SBMLNotAnnotated().strModel();
        } else {
            throw std::invalid_argument("ModelType is not a valid argument.");
        }
    }
}

