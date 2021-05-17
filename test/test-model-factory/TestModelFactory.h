//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_TESTMODELFACTORY_H
#define ROADRUNNER_TESTMODELFACTORY_H

#include "TestModelInterface.h"
#include "Brown2004.h"
#include "CeilInRateLaw.h"
#include "FactorialInRateLaw.h"
#include "Model28.h"
#include "Model269.h"
#include "OpenLinearFlux.h"
#include "SimpleFlux.h"
#include "SimpleFluxManuallyReduced.h"
#include "Venkatraman2010.h"
#include "LayoutOnly.h"

/**
 * @brief returns a vector of strings that are the
 * names of the classes that inherit from TestModel.
 */
std::vector<std::string> getAvailableTestModels();

/**
 * @brief Create instances of TestModel
 * @param modelName the name of the model to create as a string
 * @see getAvailableTestModels for list of available models.
 */
TestModel *TestModelFactory(const std::string &modelName);



#endif //ROADRUNNER_TESTMODELFACTORY_H
