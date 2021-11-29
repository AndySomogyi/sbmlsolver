//
// Created by Ciaran on 29/11/2021.
//

#include "rrRoadRunner.h"
#include "rrConfig.h"
//#include "../TestModelFactory.h"
#include "gtest/gtest.h"
#include <filesystem>

using namespace rr;

class BuildAModel : public ::testing::Test{
public:
    BuildAModel() = default;
};

/**
 * Threading is limited by disk reads, like from sbml file through
 * libsbml and to roadrunner.
 * Can we improve on this bottleneck by reading from copies
 */

TEST_F(BuildAModel, build){
    Config::setValue(Config::LLVM_BACKEND, Config::LLJIT);
    Config::setValue(Config::LOADSBMLOPTIONS_RECOMPILE, true);
    Config::setValue(Config::LLJIT_NUM_THREADS, 15);

    std::string fname = R"(D:\roadrunner\roadrunner\test\PerformanceTests\models\LargeHanekomNetworks\0.sbml)";
    for (int i=0; i<10; i++){
        RoadRunner rr(fname);
    }
}
