//
// Created by Ciaran on 10/03/2021.
//

#include "SVD.h"
#include "rrException.h"

#include "rr-libstruct/lsMatrix.h"
#include <array>
#include <iostream>


namespace rr {

    void print_matrix(char *desc, int m, int n, doublereal *a, int lda) {
        int i, j;
        printf("\n %s\n", desc);
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++) printf(" %6.2f", a[i + j * lda]);
            printf("\n");
        }
    }

    SVD::SVD(ls::DoubleMatrix &matrix) : inputMatrix_(matrix) {
        /**
         * The lapack rountine at the core of this class
         * seems to expect the transpose of our input matrix
         * so we do that here in the constructor.
         */
        inputMatrix_ = *inputMatrix_.getTranspose();
        nRows_ = (int) inputMatrix_.numRows();
        nCols_ = (int) inputMatrix_.numCols();
        lda_ = (int) inputMatrix_.numRows();
        ldu_ = (int) inputMatrix_.numRows();
        ldvt_ = (int) inputMatrix_.numCols();
        singularValues_.resize(1, min(nRows_, nCols_));
        leftSingularVectors_.resize(nRows_, nRows_);
        rightSingularVectors_.resize(nCols_, nCols_);
        compute();
    };

    /**
     * Example program:
     *  https://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/dgesvd_ex.c.html
     */
    void SVD::compute() {
        /* Locals */
        integer m = nRows_, n = nCols_, lda = nRows_, ldu = nRows_, ldvt = nCols_, info, lwork;
        doublereal wkopt;
        doublereal *work;
        /* Local arrays */
        doublereal s[5], u[6 * 6], vt[5 * 5];
        doublereal a[5 * 6] = {
                8.79, 6.11, -9.15, 9.57, -3.49, 9.84,
                9.93, 6.91, -7.93, 1.64, 4.02, 0.15,
                9.83, 5.04, 4.86, 8.83, 9.80, -8.99,
                5.45, -0.27, 4.85, 0.74, 10.00, -6.02,
                3.16, 7.98, 3.01, 5.80, 4.27, -5.31
        };
//    doublereal a[LDA * N] = {
//            8.79, 9.93, 9.83, 5.45, 3.16,
//            6.11, 6.91, 5.04, -0.27, 7.98,
//            -9.15, -7.93, 4.86, 4.85, 3.01,
//            9.57, 1.64, 8.83, 0.74, 5.80,
//            -3.49, 4.02, 9.80, 10.00, 4.27,
//            9.84, 0.15, -8.99, -6.02, -5.31,
//    };
        /* Executable statements */
        printf(" DGESVD Example Program Results\n");
        /* Query and allocate the optimal workspace */
        lwork = -1;
        dgesvd_("All", "All", &m, &n, a, &lda, singularValues_.getArray(), u, &ldu, vt, &ldvt, &wkopt, &lwork, &info);
        lwork = (int) wkopt;
        work = (doublereal *) malloc(lwork * sizeof(doublereal));
        /* Compute SVD */
        dgesvd_("All", "All", &m, &n, a, &lda, singularValues_.getArray(), u, &ldu, vt, &ldvt, work, &lwork, &info);
        /* Check for convergence */
        if (info > 0) {
            printf("The algorithm computing SVD failed to converge.\n");
            exit(1);
        }
        /* Print singular values */
        print_matrix("Singular values", 1, n, singularValues_.getArray(), 1);
        /* Print left singular vectors */
        print_matrix("Left singular vectors (stored columnwise)", m, n, u, ldu);
        /* Print right singular vectors */
        print_matrix("Right singular vectors (stored rowwise)", n, n, vt, ldvt);
        /* Free workspace */
        free((void *) work);

//        integer info, lwork;
//        doublereal wkopt;
//        doublereal *work;
//
//        doublereal a[(5 * 6)] = {
//                8.79, 6.11, -9.15, 9.57, -3.49, 9.84,
//                9.93, 6.91, -7.93, 1.64, 4.02, 0.15,
//                9.83, 5.04, 4.86, 8.83, 9.80, -8.99,
//                5.45, -0.27, 4.85, 0.74, 10.00, -6.02,
//                3.16, 7.98, 3.01, 5.80, 4.27, -5.31
//        };
//        /* Query and allocate the optimal workspace */
//        lwork = -1;
//
//
//        dgesvd_((char *) "All",  // char *jobu
//                (char *) "All",  // char *jobvt
//                &nRows_,     // integer *m
//                &nCols_,     // integer *n
//                a,//inputMatrix_.getArray(),      // doublereal *a (aka double)
//                &lda_,   // integer *lda
//                singularValues_.getArray(),     // doublereal *s
//                leftSingularVectors_.getArray(),     // doublereal *u
//                &ldu_,   // integer * ldu
//                rightSingularVectors_.getArray(),    // doublereal *vt
//                &ldvt_,  // integer *ldvt
//                &wkopt, // doublereal *wor
//                &lwork, // integer *lwork
//                &info   // integer *info
//        );
//        lwork = (int) wkopt;
//        work = (doublereal *) malloc(lwork * sizeof(doublereal));
//
////        lwork = (int) wkopt;
////        work = new doublereal[lwork];
//        /* Compute SVD */
//        dgesvd_((char *) "All",  // char *jobu
//                (char *) "All",  // char *jobvt
//                &nRows_,     // integer *m
//                &nCols_,     // integer *n
//                inputMatrix_.getArray(),      // doublereal *a (aka double)
//                &lda_,   // integer *lda
//                singularValues_.getArray(),     // doublereal *s
//                leftSingularVectors_.getArray(),     // doublereal *u
//                &ldu_,   // integer * ldu
//                rightSingularVectors_.getArray(),    // doublereal *vt
//                &ldvt_,  // integer *ldvt
//                work, // doublereal *wor
//                &lwork, // integer *lwork
//                &info   // integer *info
//        );
//        /* Check for convergence */
//        if (info > 0) {
//            throw DidNotConvergeException("The algorithm computing SVD failed to converge.\n");
//        }
//
//        //std::vector<double> tmpSingularValues(s);
////        singularValues_ = ls::DoubleMatrix (s, 1, n_);
////        leftSingularVectors_ = ls::DoubleMatrix (u, n_, m_);
////        rightSingularVectors_ = ls::DoubleMatrix (vt, n_, n_);
////
////        std::cout << "singularValues_: " << singularValues_ << std::endl;
////        std::cout << "leftSingularVectors_: " << leftSingularVectors_ << std::endl;
////        std::cout << "rightSingularVectors_: " << rightSingularVectors_ << std::endl;
//
//        /* Free workspace */
////        delete[] work;
    }

    const ls::DoubleMatrix &SVD::getSingularValues() const {
        return singularValues_;
    }

    const ls::DoubleMatrix &SVD::getLeftSingularVectors() const {
        return leftSingularVectors_;
    }

    const ls::DoubleMatrix &SVD::getRightSingularVectors() const {
        return rightSingularVectors_;
    }
}