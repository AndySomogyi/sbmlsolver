//
// Created by Ciaran on 24/03/2021.
//

#ifndef ROADRUNNER_SUNDIALSSTEADYSTATESOLVERUNITTEST_H
#define ROADRUNNER_SUNDIALSSTEADYSTATESOLVERUNITTEST_H

#include <fstream>
#include <rr-libstruct/lsLibStructural.h>
#include "gtest/gtest.h"

#include "NewtonIteration.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"

using namespace rr;

/**
 * @brief Base class for unit testing steady state solvers
 * from sundials suite.
 *
 * @details unit tests will only test the api is working correctly.
 * We aren't too fussed about simulation results here since those
 * are rigerously tested under integration testing. Therefore, only
 * one model is used, the simplest.
 */
class SundialsSteadyStateSolverUnitTest : public ::testing::Test {
public:
    SimpleFluxManuallyReduced testModel;
    RoadRunner *rr;

    SundialsSteadyStateSolverUnitTest();

    ~SundialsSteadyStateSolverUnitTest() override;

    void checkResults(ls::DoubleMatrix actualResult);

    static void checkResults(ls::DoubleMatrix expectedMatrix, ls::DoubleMatrix actualMatrix);

    static void checkResults(StringDoubleMap expectedMap, ls::DoubleMatrix result);
};


#endif //ROADRUNNER_SUNDIALSSTEADYSTATESOLVERUNITTEST_H
