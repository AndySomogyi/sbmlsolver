//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_BROWN2004_H
#define ROADRUNNER_BROWN2004_H

#include "TestModelFactory.h"


class Brown2004 : public TestModel, public SteadyStateResult {

public:

    std::string str() override;

    std::string modelName() override;

    StringDoubleMap steadyState() override;

    std::unordered_map<std::string, rr::Setting> steadyStateSettings() override;

};



#endif //ROADRUNNER_BROWN2004_H
