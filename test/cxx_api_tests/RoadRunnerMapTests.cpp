//
// Created by Ciaran on 09/12/2021.
//

#include "gtest/gtest.h"
#include "rrRoadRunnerMap.h"
#include "TestModelFactory.h"
using namespace rr;

class RoadRunnerMapTests : public ::testing::Test {
public:
    std::vector<std::string> sbml;
    RoadRunnerMapTests() {
        Logger::setLevel(Logger::LOG_DEBUG);
        std::vector<std::string> input({
            OpenLinearFlux().str(),
            SimpleFlux().str(),
            Brown2004().str(),
            Venkatraman2010().str()
        });
        sbml.swap(input);
    }
};


TEST_F(RoadRunnerMapTests, c){
    RoadRunnerMap rrm(sbml, 2);
}























