//
// Created by Ciaran on 17/12/2021.
//

#include "gtest/gtest.h"
#include "STS.h"

/**
 * A little contrived to test the interface to a test suite,
 * but it needs doing...
 */

class SemanticSTSModelTests : public ::testing::Test {
public:
    SemanticSTSModelTests() = default;
};

TEST_F(SemanticSTSModelTests, getRootDirectory) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getRootDirectory();
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getModelDescriptionFile) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getModelDescriptionFile();
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getResultsFile) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getResultsFile();
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getSettingsFile) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getSettingsFile();
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getL3V2) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getLevelAndVersion(3, 2);
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getL3V1) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getLevelAndVersion(3, 1);
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getL2V5) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getLevelAndVersion(2, 5);
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getL2V4) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getLevelAndVersion(2, 4);
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getL2V3) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getLevelAndVersion(2, 3);
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(SemanticSTSModelTests, getL2V2) {
    SemanticSTSModel s(3);
    std::filesystem::path p = s.getLevelAndVersion(2, 2);
    ASSERT_TRUE(std::filesystem::exists(p));
}

class StochasticSTSModelTests : public ::testing::Test {
public:
    StochasticSTSModelTests() = default;
};


TEST_F(StochasticSTSModelTests, getMeanFile) {
    StochasticSTSModel s(3);
    std::filesystem::path p = s.getMeanFile();
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(StochasticSTSModelTests, getSDFile) {
    StochasticSTSModel s(3);
    std::filesystem::path p = s.getSDFile();
    ASSERT_TRUE(std::filesystem::exists(p));
}

TEST_F(StochasticSTSModelTests, getModFile) {
    StochasticSTSModel s(3);
    std::filesystem::path p = s.getModFile();
    ASSERT_TRUE(std::filesystem::exists(p));
}


class STSTests : public ::testing::Test {
public:
    STSTests() = default;
};

TEST_F(STSTests, getModelNFromSTS){
    STS<SemanticSTSModel> sts;
    SemanticSTSModel stsModel = sts.getModelNFromSTS(5);
    ASSERT_TRUE(std::filesystem::exists(stsModel.getLevelAndVersion(3, 2)));
}

TEST_F(STSTests, getModelsFromSTSVec){
    std::vector<int> v ({6, 3, 2, 66, 33});
    STS<SemanticSTSModel> sts;
    std::vector<std::string> models = sts.getModelsFromSTS(v);
    ASSERT_EQ(v.size(), models.size());
}

TEST_F(STSTests, getModelsFromSTS){
    STS<SemanticSTSModel> sts;
    std::vector<std::string> models = sts.getModelsFromSTS(1, 10);
    ASSERT_EQ(9, models.size());
}

