//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_VENKATRAMAN2010_H
#define ROADRUNNER_VENKATRAMAN2010_H

#include "TestModelInterface.h"


/**
 * Model from the Venkatraman 2010 paper
 */
class Venkatraman2010 : public TestModel, public SteadyStateResult {
public:

    std::string str() override;

    std::string modelName() override;

    StringDoubleMap steadyState() override;

    std::unordered_map<std::string, rr::Setting> steadyStateSettings() override;

};


#endif //ROADRUNNER_VENKATRAMAN2010_H
