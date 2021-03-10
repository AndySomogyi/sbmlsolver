//
// Created by Ciaran on 10/03/2021.
//

#ifndef ROADRUNNER_SVD_H
#define ROADRUNNER_SVD_H

#include "rr-libstruct/lsMatrix.h"
extern "C" {
    // this one cost me a few hours. Name mangling causes link errors
    #include "clapack/f2c.h" // must be included before clapack
    #include "clapack/clapack.h"
    #include "clapack/blaswrap.h"
}
namespace rr {

    /**
     * @brief The routine computes the singular value decomposition
     * (SVD) of a real m-by-n matrix.
     * @details Often when computing the steady state of a system the
     * jacobian matrix is singular. SVD is a way of computing the
     * rank of a matrix, which in turn can be used to determine
     * whether a rank is singular:
     *      - if rank < min(N, M) of an NxM matrix then it is singular.
     * SVD uses xGESDD from clapack, the same routine used by numpy.
     * @see https://scicomp.stackexchange.com/questions/1861/understanding-how-numpy-does-svd/1863#1863
     *
     */
    class SVD {

    public:

        explicit SVD(ls::DoubleMatrix& matrix);

        const ls::DoubleMatrix &getSingularValues() const;

        const ls::DoubleMatrix &getLeftSingularVectors() const;

        const ls::DoubleMatrix &getRightSingularVectors() const;

    private:

        void compute();

        ls::DoubleMatrix& inputMatrix_;
        integer nRows_;
        integer nCols_;
        integer lda_;
        integer ldu_;
        integer ldvt_;
//        integer info{}, lwork{};
//        doublereal wkopt{};
//        doublereal *work = nullptr;

        ls::DoubleMatrix singularValues_;
        ls::DoubleMatrix leftSingularVectors_;
        ls::DoubleMatrix rightSingularVectors_;

    };
}


#endif //ROADRUNNER_SVD_H
