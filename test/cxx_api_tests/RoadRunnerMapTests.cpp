//
// Created by Ciaran on 09/12/2021.
//
#include "STS.h"
#include "rrRoadRunnerMap.h"
#include "rrConfig.h"
#include <filesystem>
#include "gtest/gtest.h"
#include "TestModelFactory.h"

using namespace rr;


/**
 * We can install VTune profiler in the cloud. woop.
 * https://www.intel.com/content/www/us/en/develop/documentation/vtune-help/top/set-up-analysis-target/targets-in-a-cloud-environment.html
 */


/*
 * if one thread is accessing the data structure through a
 * particular function, which functions are safe to call
 * from other threads?
 *
 * Which methods need read safety and which need write safety?
 */

class RoadRunnerMapTests : public ::testing::Test {
public:
    STS<SemanticSTSModel> sts;
    std::vector<std::string> sbmlStrings;
    /**
     * number of sbml files to use. You can change this number
     * without affecting the accuracy of the tests.
     * Kept low by default for running the tests quickly on azure.
     */
    int N = 5;

    RoadRunnerMapTests() {
        Config::setValue(Config::LLVM_BACKEND, Config::LLJIT);
        // better to get the strings, otherwise we risk trying to
        // read from the same file multiple from multiple threads at
        // the same time
        sbmlStrings = sts.getModelsFromSTSAsStrings(1, N + 1);
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
    RoadRunnerMap rrm(sbmlStrings, 1);
    std::vector<std::string> actual = rrm.getKeys();
    auto expectedKeys = getExpectedKeys();
    /**
    * The order of getKeys() is not guarenteed to be the same each time
    * because of multithreading, so we sort before making the comparison.
    */
    std::sort(expectedKeys.begin(), expectedKeys.end());
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expectedKeys, actual);
}

TEST_F(RoadRunnerMapTests, size) {
    RoadRunnerMap rrm(sbmlStrings, 1);
    auto keys = rrm.getKeys();
    ASSERT_EQ(N, rrm.size());
}

TEST_F(RoadRunnerMapTests, InsertUniquePtrToRRModel_Size) {
    RoadRunnerMap rrm(sbmlStrings, 1);
    ASSERT_EQ(N, rrm.size());
    std::unique_ptr<RoadRunner> rr = std::make_unique<RoadRunner>(sts.getModelNFromSTS(50, 2, 4));
    rrm.insert(std::move(rr));
    ASSERT_EQ(N + 1, rrm.size());
}


TEST_F(RoadRunnerMapTests, InsertUniquePtrToRRModel_CustomKey) {
    RoadRunnerMap rrm(sbmlStrings, 1);
    std::unique_ptr<RoadRunner> rr = std::make_unique<RoadRunner>(sts.getModelNFromSTS(50, 2, 4));
    rrm.insert("MyKey", std::move(rr));
    ASSERT_TRUE(rrm.count("MyKey"));
}

TEST_F(RoadRunnerMapTests, InsertRRModelFromSBMLFile) {
    RoadRunnerMap rrm(sbmlStrings, 1);
    rrm.insert(sts.getModelNFromSTS(50, 2, 4));
    auto expected = getExpectedKeys();
    expected.push_back("case00050");
    auto actual = rrm.getKeys();
    std::sort(expected.begin(), expected.end());
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
}

TEST_F(RoadRunnerMapTests, InsertRRModelFromSBMLFile_WithCustomKey) {
    RoadRunnerMap rrm(sbmlStrings, 1);
    rrm.insert("MyKey", sts.getModelNFromSTS(50, 2, 4));
    auto expected = getExpectedKeys();
    expected.push_back("MyKey");
    auto actual = rrm.getKeys();
    std::sort(expected.begin(), expected.end());
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
}

TEST_F(RoadRunnerMapTests, InsertFromVectorOfsbmlStrings_Size) {
    RoadRunnerMap rrm(sbmlStrings, 2);
    const std::vector<std::string> &more = sts.getModelsFromSTSAsStrings(N+1, N+1+N);
    rrm.insert(more);
    ASSERT_EQ(N * 2, rrm.size());
}

TEST_F(RoadRunnerMapTests, InsertFromVectorOfsbmlStrings_Keys) {
    // This test will break if N = NumTestsInSBMLTestSuite / 2;
    RoadRunnerMap rrm(sbmlStrings, 1);
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
    RoadRunnerMap rrm(sbmlStrings, 1);
    ASSERT_EQ(N, rrm.size());
    std::ostringstream thirdCase;
    rrm.erase("case00001");
    ASSERT_EQ(N - 1, rrm.size());
}

TEST_F(RoadRunnerMapTests, Remove_CorrectModelRemoved) {
    RoadRunnerMap rrm(sbmlStrings, 1);
    auto it = rrm.find("case00001");
    // first make sure key is in the map
    ASSERT_NE(it, rrm.end());
    // now remove it and check that it is not in the map
    rrm.erase("case00001");
    it = rrm.find("case00001");
    ASSERT_EQ(it, rrm.end());
}

TEST_F(RoadRunnerMapTests, RangeBasedLoop) {
    RoadRunnerMap rrm(sbmlStrings, 2);
    // RoadRunner obj is unqiue ptr, so we must use a reference here
    std::vector<int> actual(N);
    int caseId = 1;
    int idx = caseId - 1;
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
    RoadRunnerMap rrm(sbmlStrings, 3);
    // RoadRunner obj is unqiue ptr, so we must use a reference here
    std::vector<int> actual(sbmlStrings.size());
    int i = 0;
    for (auto& [modelName, rr] : rrm) {
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
    std::sort(actual.begin(), actual.end());
    std::sort(expected.begin(), expected.end());
    ASSERT_EQ(expected, actual);
}

TEST_F(RoadRunnerMapTests, EmptyWhenTrue) {
    RoadRunnerMap rrm;
    ASSERT_TRUE(rrm.empty());
}

TEST_F(RoadRunnerMapTests, EmptyWhenFalse) {
    RoadRunnerMap rrm(sbmlStrings, 3);
    ASSERT_FALSE(rrm.empty());
}

TEST_F(RoadRunnerMapTests, Clear) {
    RoadRunnerMap rrm(sbmlStrings, 3);
    ASSERT_FALSE(rrm.empty());
    rrm.clear();
    ASSERT_TRUE(rrm.empty());
}

/**
 * The getter operator [] returns a borrowed reference. It is still owned by the RoadRunnerMap
 */
TEST_F(RoadRunnerMapTests, GetterOperator_CheckThatWeGetBorrowedReference) {
    RoadRunnerMap rrm(sbmlStrings, 3);
    unsigned int original = rrm.size();
    RoadRunner *rr = rrm["case00001"];
    unsigned int now = rrm.size();
    ASSERT_EQ(original, now);
}

TEST_F(RoadRunnerMapTests, Find_WhenKeyIsInMap) {
    RoadRunnerMap rrm(sbmlStrings, 3);
    auto it = rrm.find("case00001");
    ASSERT_NE(it, rrm.end());
}

TEST_F(RoadRunnerMapTests, Find_WhenKeyIsNotInMap) {
    RoadRunnerMap rrm(sbmlStrings, 3);
    auto it = rrm.find("NotInMap");
    ASSERT_EQ(it, rrm.end());
}

TEST_F(RoadRunnerMapTests, CountWhenTrue) {
    RoadRunnerMap rrm(sbmlStrings, 3);
    ASSERT_TRUE(rrm.count("case00001"));
}

TEST_F(RoadRunnerMapTests, CountWhenFalse) {
    RoadRunnerMap rrm(sbmlStrings, 3);
    ASSERT_FALSE(rrm.count("NotInMap"));
}



TEST_F(RoadRunnerMapTests, Request0Threads) {
    ASSERT_THROW(
            RoadRunnerMap rrm(sbmlStrings, 0);,
            std::invalid_argument
    );
}





















