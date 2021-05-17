//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_CEILINRATELAW_H
#define ROADRUNNER_CEILINRATELAW_H


#include "TestModelInterface.h"


/**
 * A model that uses "ceil" in the rate law
 */
class CeilInRateLaw : public TestModel, public TimeSeriesResult {
public:

    std::string str() override;

    std::string modelName() override;

    ls::DoubleMatrix timeSeriesResult() override;

    std::unordered_map<std::string, rr::Setting> timeSeriesSettings() override;

};



#endif //ROADRUNNER_CEILINRATELAW_H
