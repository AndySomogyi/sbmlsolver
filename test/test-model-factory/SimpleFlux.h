//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_SIMPLEFLUX_H
#define ROADRUNNER_SIMPLEFLUX_H

#include "TestModelInterface.h"


/**
 * A -> B; k1
 * B -> A; k2
 * k1 = 0.5
 * k2 = 1.0
 * A = 10;
 * B = 1;
 */
class SimpleFlux :
        public TestModel,
        public TimeSeriesResult,
        public SteadyStateResult {
public:

    std::string str() override;

    std::string modelName() override;

    ls::DoubleMatrix timeSeriesResult() override;

    StringDoubleMap steadyState() override;

    std::unordered_map<std::string, rr::Setting> steadyStateSettings() override;

    std::unordered_map<std::string, rr::Setting> timeSeriesSettings() override;

};


#endif //ROADRUNNER_SIMPLEFLUX_H
