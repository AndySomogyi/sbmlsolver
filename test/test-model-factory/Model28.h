//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_MODEL28_H
#define ROADRUNNER_MODEL28_H


#include "TestModelInterface.h"



/**
 * model 28 from the sbml test suite
 */
class Model28 : public TestModel, public TimeSeriesResult {
public:

    std::string str() override;

    std::string modelName() override;

    ls::DoubleMatrix timeSeriesResult() override;

    std::unordered_map<std::string, rr::Setting> timeSeriesSettings() override;

};



#endif //ROADRUNNER_MODEL28_H
