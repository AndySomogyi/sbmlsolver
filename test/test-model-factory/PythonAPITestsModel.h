//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_PYTHONAPITESTSMODEL_H
#define ROADRUNNER_PYTHONAPITESTSMODEL_H

#include "TestModelInterface.h"

/**
 * @brief this is the model used in python_api_tests.py
 * @details The tests in python_api_tests.py do not test
 * simulation output. They are unit tests that test the
 * front facing roadrunner API
 */
class PythonAPITestsModel : public TestModel {
public:
    std::string str() override;

    std::string modelName() override;

};


#endif //ROADRUNNER_PYTHONAPITESTSMODEL_H
