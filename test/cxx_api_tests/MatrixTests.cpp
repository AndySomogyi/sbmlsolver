//
// Created by Ciaran on 08/07/2021.
//

#include "gtest/gtest.h"
#include "Matrix.h"

using namespace rr;

class MatrixTests : public ::testing::Test {
public:
    MatrixTests() = default;
};


TEST_F(MatrixTests, TestEqualityWithIntsTrue){
    Matrix<int> first({
                              {1, 2, 3},
                              {4, 5, 6},
    });
    Matrix<int> second({
                              {1, 2, 3},
                              {4, 5, 6},
    });
    ASSERT_TRUE(first == second);
}

TEST_F(MatrixTests, TestEqualityWithIntsFalse){
    Matrix<int> first({
                              {1, 2, 3},
                              {4, 5, 7},
    });
    Matrix<int> second({
                              {1, 2, 3},
                              {4, 5, 6},
    });
    ASSERT_TRUE(first != second);
}

TEST_F(MatrixTests, TestAlmostEqualsWhenTrue){
    Matrix<double> first({
                              {1.1234, 2.1234, 3.1234},
                              {4.1234, 5.1234, 7.1234},
    });
    Matrix<double> second({
                              {1.1234, 2.1234, 3.1234},
                              {4.1234, 5.1234, 7.1234},
    });
    ASSERT_TRUE(first.almostEquals(second, 1e-4));
}


TEST_F(MatrixTests, TestAlmostEqualsWhenFalse){
    Matrix<double> first({
                              {1.1245, 2.1234, 3.1234},
                              {4.1234, 5.1234, 7.1234},
    });
    Matrix<double> second({
                              {1.1234, 2.1234, 3.1234},
                              {4.1234, 5.1234, 7.1234},
    });
    ASSERT_FALSE(first.almostEquals(second, 1e-5));
}

TEST_F(MatrixTests, TestAlmostEqualsWhenFalseButAcceptableTolerance){
    Matrix<double> first({
                              {1.12346, 2.1234, 3.1234},
                              {4.1234, 5.1234, 7.1234},
    });
    Matrix<double> second({
                              {1.1234, 2.1234, 3.1234},
                              {4.1234, 5.1234, 7.1234},
    });
    ASSERT_TRUE(first.almostEquals(second, 1e-3));
}


TEST_F(MatrixTests, CheckEmptyWhenTrue){
    Matrix<double> emptyMatrix;
    ASSERT_TRUE(emptyMatrix.empty());
}

TEST_F(MatrixTests, CheckEmptyWhenTrueWithColnames){
    Matrix<double> emptyMatrix;
    emptyMatrix.setColNames({"S1", "S2"});
    ASSERT_TRUE(emptyMatrix.empty());
}

TEST_F(MatrixTests, CheckEmptyWhenFalse){
    Matrix<double> notEmptyMatrix({
                                       {0, 1, 2}
    });
    ASSERT_FALSE(notEmptyMatrix.empty());
}

TEST_F(MatrixTests, CheckEqualityOfEmptyMatrices){
    Matrix<double> first;
    Matrix<double> second;
    bool equal = first == second;
    ASSERT_TRUE(equal);
}

TEST_F(MatrixTests, ConstructorForLsMatrix){
    ls::Matrix<int> lsMatrix({
                                        {1, 2}
    });
    rr::Matrix<int> rrMatrix(lsMatrix);
    rr::Matrix<int> expected({{1, 2}});
    ASSERT_TRUE(expected == rrMatrix);
}

TEST_F(MatrixTests, ConstructorForLsMatrixWithRowNames){
    ls::Matrix<int> lsMatrix({
                                        {1, 2}
    });
    lsMatrix.setRowNames({"R0","R1" });
    rr::Matrix<int> rrMatrix(lsMatrix);
    std::vector<std::string> expectedColNames({"R0", "R1"});
    ASSERT_TRUE(expectedColNames == rrMatrix.getRowNames());
}

TEST_F(MatrixTests, ConstructorForLsMatrixWithColNames){
    ls::Matrix<int> lsMatrix({{1, 2}});
    lsMatrix.setRowNames({"R0","R1" });
    lsMatrix.setColNames({"C0","C1" });
    rr::Matrix<int> rrMatrix(lsMatrix);
    std::vector<std::string> expectedColNames({"C0", "C1"});
    ASSERT_TRUE(expectedColNames == rrMatrix.getColNames());
}


















