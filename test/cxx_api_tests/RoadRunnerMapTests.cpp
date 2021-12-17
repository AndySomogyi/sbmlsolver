//
// Created by Ciaran on 09/12/2021.
//
#include "STS.h"
#include "rrRoadRunnerMap.h"
#include "rrConfig.h"
#include <filesystem>
#include "gtest/gtest.h"

using namespace rr;


/**
 * We can install VTune profiler in the cloud. woop.
 * https://www.intel.com/content/www/us/en/develop/documentation/vtune-help/top/set-up-analysis-target/targets-in-a-cloud-environment.html
 */

class RoadRunnerMapTests : public ::testing::Test {
public:
    STS<SemanticSTSModel> sts;
    std::vector<std::string> sbmlFiles;
    /**
     * number of sbml files to use. You can change this number
     * without affecting the accuracy of the tests.
     * Kept low by default for running the tests quickly on azure.
     */
    int N = 5;

    RoadRunnerMapTests() {
        sbmlFiles = sts.getModelsFromSTS(1, N+1);
    }

    std::vector<std::string> getExpectedKeys() const {
        std::vector<std::string> expected(N);
        for (int caseID = 1; caseID <= N; caseID++) {
            int idx = caseID - 1;
            std::ostringstream os;
            os << std::setfill('0') << std::setw(5) << caseID;
            expected[idx] = "case" + os.str();
        }
        return expected;
    }
};

/**
 * Use case00001 where possible since it is guarenteed to exist, unless a RoadRunnerMap
 * is default constructed.
 */

TEST_F(RoadRunnerMapTests, getKeys) {
    Config::setValue(Config::LLVM_BACKEND, Config::MCJIT);
    RoadRunnerMap rrm(sbmlFiles, 1);
    auto keys = rrm.getKeys();
    auto expectedKeys = getExpectedKeys();
    /**
    * The order of getKeys() is not guarenteed to be the same each time
    * because of multithreading, so we sort before making the comparison.
    */
    std::vector<std::string> actual = rrm.getKeys();
    std::sort(expectedKeys.begin(), expectedKeys.end());
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expectedKeys, keys);
}

TEST_F(RoadRunnerMapTests, size) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    auto keys = rrm.getKeys();
    ASSERT_EQ(N, rrm.size());
}

TEST_F(RoadRunnerMapTests, InsertUniquePtrToRRModel_Size) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    std::unique_ptr<RoadRunner> rr = std::make_unique<RoadRunner>(sts.getModelNFromSTS(50, 2, 4));
    rrm.insert(std::move(rr));
    ASSERT_EQ(N + 1, rrm.size());
}

TEST_F(RoadRunnerMapTests, InsertUniquePtrToRRModel_KeysUpdated) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    std::unique_ptr<RoadRunner> rr = std::make_unique<RoadRunner>(sts.getModelNFromSTS(50, 2, 4));
    rrm.insert(std::move(rr));
    ASSERT_STREQ("case00050", rrm.getKeys()[rrm.size() - 1].c_str());
}

TEST_F(RoadRunnerMapTests, InsertUniquePtrToRRModel_CustomKey) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    std::unique_ptr<RoadRunner> rr = std::make_unique<RoadRunner>(sts.getModelNFromSTS(50, 2, 4));
    rrm.insert("MyKey", std::move(rr));
    ASSERT_STREQ("MyKey", rrm.getKeys()[rrm.size() - 1].c_str());
}

TEST_F(RoadRunnerMapTests, InsertRRModelFromSBMLFile) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    rrm.insert(sts.getModelNFromSTS(50, 2, 4));
    ASSERT_STREQ("case00050", rrm.getKeys()[rrm.size() - 1].c_str());
}

TEST_F(RoadRunnerMapTests, InsertRRModelFromSBMLFile_WithCustomKey) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    rrm.insert("MyKey", sts.getModelNFromSTS(50, 2, 4));
    ASSERT_STREQ("MyKey", rrm.getKeys()[rrm.size() - 1].c_str());
}

TEST_F(RoadRunnerMapTests, InsertFromVectorOfSBMLFiles_Size) {
    RoadRunnerMap rrm(sbmlFiles, 2);
    const std::vector<std::string> &more = sts.getModelsFromSTS(N+1, N+1+N);
    rrm.insert(more);
    ASSERT_EQ(N * 2, rrm.size());
}


TEST_F(RoadRunnerMapTests, InsertFromVectorOfSBMLFiles_Keys) {
    // This test will break if N = NumTestsInSBMLTestSuite / 2;
    RoadRunnerMap rrm(sbmlFiles, 1);
    const std::vector<std::string> &more = sts.getModelsFromSTS(N+1, N+1+N);
    rrm.insert(more);
    std::vector<std::string> expected(N*2);
    int idx = 0;
    for (int caseID = 1; caseID <= N; caseID++) {
        std::ostringstream os;
        os << std::setfill('0') << std::setw(5) << caseID;
        expected[idx++] = "case" + os.str();
    }
    for (int caseID = N+1; caseID < N+1+N; caseID++) {
        std::ostringstream os;
        os << std::setfill('0') << std::setw(5) << caseID;
        expected[idx++] = "case" + os.str();
    }

    /**
     * The order of getKeys() is not guarenteed to be the same each time
     * because of multithreading, so we sort before making the comparison.
     */
    std::vector<std::string> actual = rrm.getKeys();
    std::sort(expected.begin(), expected.end());
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
}


TEST_F(RoadRunnerMapTests, Remove_Size) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    ASSERT_EQ(N, rrm.size());
    std::ostringstream thirdCase;
    // N = x + y; 
    rrm.remove("case00001");
    ASSERT_EQ(N - 1, rrm.size());
}

TEST_F(RoadRunnerMapTests, Remove_CorrectModelRemoved) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    auto it = rrm.findKey("case00001");
    // first make sure key is in the map
    ASSERT_NE(it, rrm.keysEnd());
    // access it
    RoadRunner *rr = rrm[*it];
    // now remove it and check that it is not in the map
    rrm.remove("case00001");
    it = std::find(rrm.keysBegin(), rrm.keysEnd(), "case00001");
    ASSERT_EQ(it, rrm.keysEnd());
}

TEST_F(RoadRunnerMapTests, Remove_KeysListAlsoRemoved) {
    RoadRunnerMap rrm(sbmlFiles, 1);
    auto it = rrm.findKey("case00001");
    // first make sure key is in the map
    ASSERT_NE(it, rrm.keysEnd());
    // now remove it and check that it is not in the map
    rrm.remove("case00001");
    it = rrm.findKey("case00001");
    ASSERT_EQ(it, rrm.keysEnd());
}

TEST_F(RoadRunnerMapTests, RangeBasedLoop) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    // RoadRunner obj is unqiue ptr, so we must use a reference here
    std::vector<int> actual(N);
    int caseId = 1;
    int idx = caseId-1;
    for (auto&[modelName, rr]: rrm) {
        actual[idx++] = rr->getInstanceID();
        caseId++;
    }
    /**
     * The order of ids is not guarenteed to be the same each time
     * because of multithreading, so we sort before making the comparison.
     */
    idx = 0;
    std::vector<int> expected(N);
    for (caseId = 1; caseId <= N; caseId++) {
        expected[idx++] = caseId;
    }
    std::sort(expected.begin(), expected.end());
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
}

TEST_F(RoadRunnerMapTests, IteratorBasedLoop) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    // RoadRunner obj is unqiue ptr, so we must use a reference here
    std::vector<int> actual(sbmlFiles.size());
    int i = 0;
    for (auto it = rrm.begin(); it != rrm.end(); ++it) {
        auto&[modelName, rr] = *it;
        actual[i] = rr->getInstanceID();
        i++;
    }
    /**
     * The order of ids is not guarenteed to be the same each time
     * because of multithreading, so we sort before making the comparison.
     */
    std::vector<int> expected(N);
    for (i = 1; i <= N; i++) {
        expected[i-1] = i;
    }
    ASSERT_EQ(expected, actual);
}

TEST_F(RoadRunnerMapTests, EmptyWhenTrue) {
    RoadRunnerMap rrm;
    ASSERT_TRUE(rrm.empty());
}

TEST_F(RoadRunnerMapTests, EmptyWhenFalse) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    ASSERT_FALSE(rrm.empty());
}

TEST_F(RoadRunnerMapTests, Clear) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    ASSERT_FALSE(rrm.empty());
    rrm.clear();
    ASSERT_TRUE(rrm.empty());
}

/**
 * The getter operator [] returns a borrowed reference. It is still owned by the RoadRunnerMap
 */
TEST_F(RoadRunnerMapTests, GetterOperator_CheckThatWeGetBorrowedReference) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    unsigned int original = rrm.size();
    RoadRunner *rr = rrm["case00001"];
    unsigned int now = rrm.size();
    ASSERT_EQ(original, now);
}

TEST_F(RoadRunnerMapTests, Steal_NumElements) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    std::unique_ptr<RoadRunner> rr = std::unique_ptr<RoadRunner>(rrm.steal("case00001"));
    ASSERT_EQ(N - 1, rrm.size());
}

TEST_F(RoadRunnerMapTests, Steal_NumKeysUpdated) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    std::unique_ptr<RoadRunner> rr = std::unique_ptr<RoadRunner>(rrm.steal("case00001"));
    ASSERT_EQ(N - 1, rrm.getKeys().size());
}

TEST_F(RoadRunnerMapTests, Find_WhenKeyIsInMap) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    auto it = rrm.find("case00001");
    ASSERT_NE(it, rrm.end());
}

TEST_F(RoadRunnerMapTests, Find_WhenKeyIsNotInMap) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    auto it = rrm.find("NotInMap");
    ASSERT_EQ(it, rrm.end());
}

TEST_F(RoadRunnerMapTests, CountWhenTrue) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    ASSERT_TRUE(rrm.count("case00001"));
}

TEST_F(RoadRunnerMapTests, CountWhenFalse) {
    RoadRunnerMap rrm(sbmlFiles, 3);
    ASSERT_FALSE(rrm.count("NotInMap"));
}



TEST_F(RoadRunnerMapTests, Request0Threads) {
    ASSERT_THROW(
            RoadRunnerMap rrm(sbmlFiles, 0);,
            std::invalid_argument
    );
}





















