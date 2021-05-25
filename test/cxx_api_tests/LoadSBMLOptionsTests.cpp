//
// Created by Ciaran on 25/05/2021.
//

#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrRoadRunnerOptions.h" // for LoadSBMLOptions
#include "rrConfig.h"

using namespace rr;

class LoadSBMLOptionsTests : public ::testing::Test {
public :

    LoadSBMLOptionsTests() = default;
};


TEST_F(LoadSBMLOptionsTests, d){
    Config::setValue(Config::LOADSBMLOPTIONS_CONSERVED_MOIETIES, false);
    LoadSBMLOptions options;
    std::cout << options.modelGeneratorOpt << std::endl;

    RoadRunner rr(SimpleFlux().str());

    std::cout << rr.getConservedMoietyAnalysis() << std::endl;

}













