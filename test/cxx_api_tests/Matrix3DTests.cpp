//
// Created by Ciaran on 08/07/2021.
//

#include "gtest/gtest.h"
#include "Matrix3D.h"

using namespace rr;

class Matrix3DTests : public ::testing::Test {
public:
    Matrix3DTests() = default;
};

//
//TEST_F(Matrix3DTests, TestEqualityWithIntsTrue) {
//    Matrix3D<double, double> matrix3D(
//            {0.0, 1.0},
//            // 0
//            {
//                    {0.0, 1.0},
//                    {0.0, 1.0},
//            },
//            // 1
//            {
//                    {0.0, 1.0},
//                    {0.0, 1.0},
//            }
//    );
//
//}

TEST_F(Matrix3DTests, d) {
    Matrix3D<double, double> matrix3D(
            {0.0, 1.0},
            {
                    // 0.0
                    {
                            {0.0, 1.0},
                            {0.0, 1.0}
                    },
                    // 1.0
                    {
                            {0.0, 2.0},
                            {0.0, 2.0}
                    }
            }
    );
    Matrix<double> actual =  matrix3D[0.0];
    Matrix<double> expected({
                                    {0.0, 1.0},
                                    {0.0, 1.0}
                            });
    bool passed = expected.almostEquals(actual, 1e-4);
    ASSERT_TRUE(passed);
}















