/*
 * TestRoadRunner.h
 *
 *  Created on: Jul 24, 2013
 *      Author: andy
 */

#ifndef TESTROADRUNNER_H_
#define TESTROADRUNNER_H_

#include "rrRoadRunner.h"
#include "rrTestSuiteModelSimulation.h"

namespace rr
{

class TestRoadRunner
{
public:
    TestRoadRunner(const std::string& version, int caseNumber);
    virtual ~TestRoadRunner();

    bool test();

    RoadRunner *rr;

    TestSuiteModelSimulation *simulation;

    std::string modelFilePath;
    std::string modelFileName;
    std::string settingsFileName;

    void loadSBML();

    void simulate();
};

} /* namespace rr */
#endif /* TESTROADRUNNER_H_ */
