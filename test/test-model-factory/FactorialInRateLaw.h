//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_FACTORIALINRATELAW_H
#define ROADRUNNER_FACTORIALINRATELAW_H

#include "TestModelInterface.h"



/**
 * A model that uses "Factorial" in the rate law
 */
class FactorialInRateLaw : public TestModel, public TimeSeriesResult {
public:

    std::string str() override;

    std::string modelName() override;

    ls::DoubleMatrix timeSeriesResult() override;

    std::unordered_map<std::string, rr::Setting> timeSeriesSettings() override;

};



#endif //ROADRUNNER_FACTORIALINRATELAW_H
