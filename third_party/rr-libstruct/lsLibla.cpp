/**
 * LibStruct, original author: Frank Bergmann.
 * Fixes and improvments: Totte Karsson
 */
#pragma hdrstop

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include <string.h>
#include "lsLibla.h"
#include "lsMatrix.h"
#include "lsUtils.h"

extern "C"
{
#include "f2c.h"
#include "clapack.h"

}
//---------------------------------------------------------------------------

namespace ls {

    using namespace std;
    using namespace ls;

    double gLapackTolerance = 1.0E-12;

    double getTolerance() {
        return gLapackTolerance;
    }

    void setTolerance(double dTolerance) {
        gLapackTolerance = dTolerance;
    }

    vector<Complex> getEigenValues(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getEigenValues " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        vector<Complex> oResult;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();
        integer lwork = 2 * numRows;
        integer info;

        if (numRows != numCols) {
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");
        }

        if (numRows == 0) return oResult;

        doublecomplex *A = new doublecomplex[numRows * numRows];
        memset(A, 0, sizeof(doublecomplex) * numRows * numRows);
        doublecomplex *eigVals = new doublecomplex[numRows];
        memset(eigVals, 0, sizeof(doublecomplex) * numRows);
        doublecomplex *work = new doublecomplex[lwork];
        memset(work, 0, sizeof(doublecomplex) * lwork);
        doublereal *rwork = new doublereal[lwork];
        memset(rwork, 0, sizeof(doublereal) * lwork);

        int index;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                index = (j + numRows * i);
                A[index].r = oMatrix(i, j);
            }
        }

        char job = 'N'; // do not compute eigenvectors
        zgeev_(&job, &job, &numRows, A, &numRows, eigVals, NULL, &numRows, NULL, &numRows, work, &lwork, rwork, &info);


        for (int i = 0; i < numRows; i++) {
            Complex complex(ls::RoundToTolerance(eigVals[i].r, gLapackTolerance),
                            ls::RoundToTolerance(eigVals[i].i, gLapackTolerance));
            oResult.push_back(complex);
        }

        delete[] eigVals;
        delete[] A;
        delete[] work;
        delete[] rwork;
        return oResult;

    }

    vector<ls::Complex> ZgetEigenValues(ComplexMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== ZgetEigenValues " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        vector<Complex> oResult;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();
        integer lwork = 2 * numRows;
        integer info;

        if (numRows != numCols)
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");

        doublecomplex *A = new doublecomplex[numRows * numRows];
        memset(A, 0, sizeof(doublecomplex) * numRows * numRows);
        doublecomplex *eigVals = new doublecomplex[numRows];
        memset(eigVals, 0, sizeof(doublecomplex) * numRows);
        doublecomplex *work = new doublecomplex[lwork];
        memset(work, 0, sizeof(doublecomplex) * lwork);
        doublereal *rwork = new doublereal[lwork];
        memset(rwork, 0, sizeof(doublereal) * lwork);

        int index;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                index = (j + numRows * i);

                A[index].r = real(oMatrix(j, i));
                A[index].i = imag(oMatrix(j, i));

            }
        }

        char job = 'N'; // do not compute eigenvectors
        zgeev_(&job, &job, &numRows, A, &numRows, eigVals, NULL, &numRows, NULL, &numRows, work, &lwork, rwork, &info);


        for (int i = 0; i < numRows; i++) {
            Complex complex(ls::RoundToTolerance(eigVals[i].r, gLapackTolerance),
                            ls::RoundToTolerance(eigVals[i].i, gLapackTolerance));
            oResult.push_back(complex);
        }

        delete[] eigVals;
        delete[] A;
        delete[] work;
        delete[] rwork;

        return oResult;
    }

    vector<DoubleMatrix *> getQRWithPivot(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getQRWithPivot " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;


        vector<DoubleMatrix *> oResult;

        integer row = oMatrix.numRows();
        integer col = oMatrix.numCols();

        if (row * col == 0) {
            DoubleMatrix *oMatrixQ = new DoubleMatrix(row, row);
            DoubleMatrix *oMatrixR = new DoubleMatrix(row, col);
            DoubleMatrix *oMatrixP = new DoubleMatrix(col, col);
            oResult.push_back(oMatrixQ);
            oResult.push_back(oMatrixR);
            oResult.push_back(oMatrixP);
            return oResult;
        }

        integer minRowCol = min(row, col);
        integer lwork = 16 * col;

        doublereal *A = oMatrix.getCopy(true);


        doublereal *Q = NULL;
        if (row * row) {
            Q = new doublereal[row * row];
            memset(Q, 0, sizeof(doublereal) * row * row);
        }
        doublereal *R = NULL;
        if (row * col) {
            R = new doublereal[row * col];
            memset(R, 0, sizeof(doublereal) * row * col);
        }
        doublereal *P = NULL;
        if (col * col) {
            P = new doublereal[col * col];
            memset(P, 0, sizeof(doublereal) * col * col);
        }

        //Log(lDebug5) << "before dorgqr A:\n"    << ls::print(row, col, A);
        //Log(lDebug5) << endl << endl << "Q: \n"    << ls::print(row, row, Q);
        //Log(lDebug5) << endl << endl << "R: \n"    << ls::print(row, col, R);
        //Log(lDebug5) << endl << endl << "P: \n"    << ls::print(col, col, P);

        doublereal *tau = NULL;
        if (minRowCol) {
            tau = new doublereal[minRowCol];
            memset(tau, 0, sizeof(doublereal) * minRowCol);
        }
        integer *jpvt = NULL;
        if (col) {
            jpvt = new integer[col];
            memset(jpvt, 0, sizeof(integer) * col);
        }
        doublereal *work = NULL;
        if (lwork) {
            work = new doublereal[lwork];
            memset(work, 0, lwork);
        }

        integer info;
        int out;

        //Log(lDebug5) << "Input:\n"<<ls::print(row, col, A);

        // call lapack routine dgepq3_ to generate householder reflections
        out = dgeqp3_(&row, &col, A, &row, jpvt, tau, work, &lwork, &info);

        //Log(lDebug5) << "before permutation" << endl;

        // Building permutation matrix P and
        for (int i = 0; i < col; i++) {
            size_t pos = i * col + (jpvt[i] - 1);
            if (pos < col * col)
                P[pos] = 1;
        }

        //Log(lDebug5) << "before memcpy" << endl;

        // set R to A before calling dorgqr
        memcpy(R, A, sizeof(doublereal) * row * col);

        //Log(lDebug5) << "after memcpy" << endl;

        // make R a trapezoidal matrix
        // set Q to A before calling dorgqr
        int index = 0;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < minRowCol; j++) {
                index = i + row * j;
                Q[index] = A[index];
            }

            if (i >= 1)
                for (int j = 0; j < min(i, col); j++) {
                    R[i + row * j] = 0.0;
                }
        }

        //Log(lDebug5) << "before dorgqr:\n"<<ls::print(row, col, A);
        //Log(lDebug5) << endl << endl << "Q: \n"<<ls::print(row, row, Q);
        //Log(lDebug5) << endl << endl << "R: \n"<<ls::print(row, col, R);
        //Log(lDebug5) << endl << endl << "P: \n"<<ls::print(col, col, P);


        // call routine dorgqr_ to build orthogonal matrix Q
        out = dorgqr_(&row, &row, &minRowCol, Q, &row, tau, work, &lwork, &info);

        //Log(lDebug5) << endl << endl << "Q: \n"<<ls::print(row, row, Q);
        //Log(lDebug5) << endl << endl << "R: \n"<<ls::print(row, col, R);
        //Log(lDebug5) << endl << endl << "P: \n"<<ls::print(col, col, P);

        DoubleMatrix *oMatrixQ = new DoubleMatrix(Q, row, row, true);
        RoundMatrixToTolerance(*oMatrixQ, gLapackTolerance);

        DoubleMatrix *oMatrixR = new DoubleMatrix(R, row, col, true);
        RoundMatrixToTolerance(*oMatrixR, gLapackTolerance);

        DoubleMatrix *oMatrixP = new DoubleMatrix(P, col, col, true);
        RoundMatrixToTolerance(*oMatrixP, gLapackTolerance);

        oResult.push_back(oMatrixQ);
        oResult.push_back(oMatrixR);
        oResult.push_back(oMatrixP);

        // free
        if (row * col) {
            delete[] A;
        }

        if (row * row) {
            delete[] Q;
        }

        if (row * col) {
            delete[] R;
        }

        if (col * col) {
            delete[] P;
        }

        if (tau) {
            delete[] tau;
        }

        if (jpvt) {
            delete[] jpvt;
        }

        if (work) {
            delete[] work;
        }

        return oResult;
    }

    vector<DoubleMatrix *> getQR(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getQR " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        integer row = oMatrix.numRows();
        integer col = oMatrix.numCols();
        if (row * col == 0) {
            vector<DoubleMatrix *> oResult;
            DoubleMatrix *oMatrixQ = new DoubleMatrix(row, row);
            DoubleMatrix *oMatrixR = new DoubleMatrix(row, col);
            oResult.push_back(oMatrixQ);
            oResult.push_back(oMatrixR);
            return oResult;
        }

        integer lwork = 16 * col;
        integer minRowCol = min(row, col);

        doublereal *Q = new doublereal[row * row];
        doublereal *R = new doublereal[row * col];
        doublereal *tau = new doublereal[minRowCol];
        doublereal *work = new doublereal[lwork];

        doublereal *A = (doublereal *) oMatrix.getCopy(true);


        //Log(lDebug5) << "Input:\n"<<ls::print(row, col, A);

        integer info;
        dgeqrf_(&row, &col, A, &row, tau, work, &lwork, &info);

        //Log(lDebug5) << "A: after dgeqrt)\n"<<ls::print(row, col, A);
        //Log(lDebug5) << "tau: after dgeqrt)\n"<<ls::print(1, minRowCol, tau);
        // set R to A before calling dorgqr
        memcpy(R, A, sizeof(double) * row * col);
        int index;
        for (int i = 0; i < row; i++) {
            if (i > 0)
                for (int j = 0; j < min(i, col); j++) {
                    R[i + row * j] = 0.0;
                }
            for (int j = 0; j < minRowCol; j++) {
                index = i + row * j;
                Q[index] = A[index];
            }
        }

        dorgqr_(&row, &row, &minRowCol, Q, &row, tau, work, &lwork, &info);

        ls::checkTolerance(row * row, Q, getTolerance());
        ls::checkTolerance(row * col, R, getTolerance());

        //Log(lDebug5) << endl << endl << "Q: )\n"<<ls::print(row, row, Q);
        //Log(lDebug5) << endl << endl << "R: )\n"<<ls::print(row, col, R);

        vector<DoubleMatrix *> oResult;

        DoubleMatrix *oMatrixQ = new DoubleMatrix(Q, row, row, true);
        ls::RoundMatrixToTolerance(*oMatrixQ, gLapackTolerance);

        DoubleMatrix *oMatrixR = new DoubleMatrix(R, row, col, true);
        ls::RoundMatrixToTolerance(*oMatrixR, gLapackTolerance);
        oResult.push_back(oMatrixQ);
        oResult.push_back(oMatrixR);

        delete[] A;
        delete[] Q;
        delete[] R;
        delete[] tau;
        delete[] work;

        return oResult;
    }


    DoubleMatrix *getLeftNullSpace(DoubleMatrix &oMatrixIn) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getLeftNullSpace " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;
        DoubleMatrix *oMatrix = oMatrixIn.getTranspose();
        DoubleMatrix *oMatrixResult = getRightNullSpace(*oMatrix);
        delete oMatrix;
        //return oMatrixResult;
        DoubleMatrix *oFinalMatrix = oMatrixResult->getTranspose();
        delete oMatrixResult;
        return oFinalMatrix;
    }

    DoubleMatrix *getScaledLeftNullSpace(DoubleMatrix &oMatrixIn) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getScaledLeftNullSpace " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;
        DoubleMatrix *oMatrix = oMatrixIn.getTranspose();
        DoubleMatrix *oMatrixResult = getScaledRightNullSpace(*oMatrix);
        delete oMatrix;
        //return oMatrixResult;
        DoubleMatrix *oFinalMatrix = oMatrixResult->getTranspose();
        delete oMatrixResult;
        return oFinalMatrix;

    }

    DoubleMatrix *getScaledRightNullSpace(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getScaledRightNullSpace " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;
        DoubleMatrix *oTemp = getRightNullSpace(oMatrix);
        DoubleMatrix *oTranspose = oTemp->getTranspose();
        delete oTemp;
        ls::GaussJordan(*oTranspose, gLapackTolerance);
        DoubleMatrix *oResult = oTranspose->getTranspose();
        delete oTranspose;

        ls::RoundMatrixToTolerance(oMatrix, gLapackTolerance);

        return oResult;
    }

    DoubleMatrix *getRightNullSpace(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getRightNullSpace " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;
        DoubleMatrix *oTranspose = oMatrix.getTranspose();

        integer numRows = oTranspose->numRows();
        integer numCols = oTranspose->numCols();

        // determine sizes
        integer min_MN = min(numRows, numCols);
        integer max_MN = max(numRows, numCols);
        integer lwork = 3 * min_MN * min_MN + max(max_MN, 4 * min_MN * min_MN + 4 * min_MN); // 'A'

        // allocate arrays for lapack
        doublereal *A = oTranspose->getCopy(true);
        doublereal *S = new doublereal[min_MN];
        memset(S, 0, sizeof(doublereal) * min_MN);
        doublereal *work = new doublereal[lwork];
        memset(work, 0, sizeof(doublereal) * lwork);
        doublereal *U = new doublereal[numRows * numRows];
        memset(U, 0, sizeof(doublereal) * numRows * numRows);
        doublereal *VT = new doublereal[numCols * numCols];
        memset(VT, 0, sizeof(doublereal) * numCols * numCols);
        integer *iwork = new integer[8 * min_MN];

        integer info;
        char jobz = 'A';
        dgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, U, &numRows, VT, &numCols, work, &lwork, iwork, &info);

        // now we have everything we could get, now extract the nullspace. In Matlab this would look like:
        //     [U,S,V] = svd(A');
        //     r = rank(A)
        //     Z = U(:,r+1:end)

        int rank = getRank(oMatrix);
        int nResultColumns = numRows - rank;

        DoubleMatrix *oMatrixU = new DoubleMatrix(U, numRows, numRows, true);

        //Log(lDebug5) << " SVD: U " << endl;
        ls::print(*oMatrixU);
        DoubleMatrix *oResultMatrix = new DoubleMatrix(numRows, nResultColumns);
        for (int i = 0; i < nResultColumns; i++) {
            for (int j = 0; j < numRows; j++) {
                (*oResultMatrix)(j, i) = (*oMatrixU)(j, rank + i);
            }
        }
        //Log(lDebug5) << " Nullspace: " << endl;
        ls::print(*oResultMatrix);
        delete[] S;
        delete[]
                work;
        delete[] U;
        delete[] VT;
        delete[] iwork;
        delete[] A;
        delete oTranspose;
        delete oMatrixU;

        ls::RoundMatrixToTolerance(*oResultMatrix, gLapackTolerance);
        return oResultMatrix;

    }

    int getRank(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getRank " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;
        int rank = 0;
        vector<double> oSingularVals = getSingularValsBySVD(oMatrix);

        for (unsigned int i = 0; i < oSingularVals.size(); i++) {
            if (fabs(oSingularVals[i]) > gLapackTolerance) {
                rank++;
            }
        }
        return rank;
    }

    vector<double> getSingularValsBySVD(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getSingularValsBySVD " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        vector<double> oResult;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();

        integer min_MN = min(numRows, numCols);
        integer max_MN = max(numRows, numCols);

        if (min_MN == 0) return oResult;

        integer lwork = 3 * min_MN + max(max_MN, 7 * min_MN);    // specified in dgesdd description

        doublereal *A = oMatrix.getCopy(true);
        doublereal *S = new doublereal[min_MN];
        memset(S, 0, sizeof(doublereal) * min_MN);
        doublereal *work = new doublereal[lwork];
        memset(work, 0, sizeof(doublereal) * lwork);
        integer *iwork = new integer[8 * min_MN];

        integer info;
        char jobz = 'N';
        dgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, NULL, &numRows, NULL, &numCols, work, &lwork, iwork, &info);

        for (int i = 0; i < min_MN; i++) {
            oResult.push_back(ls::RoundToTolerance(S[i], gLapackTolerance));
        }

        // free memory
        delete[] A;
        delete[] S;
        delete[] work;
        delete[] iwork;

        return oResult;
    }

    ComplexMatrix *getEigenVectors(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getEigenVectors " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();
        integer lwork = 2 * numRows;
        integer info;

        if (numRows != numCols)
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");

        if (numRows == 0) return new ComplexMatrix();

        doublecomplex *A = new doublecomplex[numRows * numRows];
        memset(A, 0, sizeof(doublecomplex) * numRows * numRows);
        doublecomplex *eigVals = new doublecomplex[numRows];
        memset(eigVals, 0, sizeof(doublecomplex) * numRows);
        doublecomplex *vr = new doublecomplex[numRows * numRows];
        memset(vr, 0, sizeof(doublecomplex) * numRows * numRows);
        doublecomplex *work = new doublecomplex[lwork];
        memset(work, 0, sizeof(doublecomplex) * lwork);
        doublereal *rwork = new doublereal[lwork];
        memset(rwork, 0, sizeof(doublereal) * lwork);

        int index;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                index = (j + numRows * i);
                A[index].r = oMatrix(j, i);
            }
        }

        char job = 'N';
        char jobR = 'V'; // compute the right eigenvectors
        zgeev_(&job, &jobR, &numRows, A, &numRows, eigVals, NULL, &numRows, vr, &numRows, work, &lwork, rwork, &info);

        ComplexMatrix *oResult = new ComplexMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                index = (j + numRows * i);
                Complex complexNr(
                        ls::RoundToTolerance(vr[index].r, gLapackTolerance),
                        ls::RoundToTolerance(vr[index].i, gLapackTolerance));

                (*oResult)(i, j) = complexNr;//(.set(complex.Real, complex.Imag);
            }
        }

        delete[] eigVals;
        delete[] A;
        delete[] work;
        delete[] rwork;
        delete[] vr;

        return oResult;
    }

    ComplexMatrix *ZgetEigenVectors(ComplexMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getEigenVectors " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();
        integer lwork = 2 * numRows;
        integer info;

        if (numRows != numCols)
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");

        if (numRows == 0) return new ComplexMatrix();

        doublecomplex *A = new doublecomplex[numRows * numRows];
        memset(A, 0, sizeof(doublecomplex) * numRows * numRows);
        doublecomplex *eigVals = new doublecomplex[numRows];
        memset(eigVals, 0, sizeof(doublecomplex) * numRows);
        doublecomplex *vr = new doublecomplex[numRows * numRows];
        memset(vr, 0, sizeof(doublecomplex) * numRows * numRows);
        doublecomplex *work = new doublecomplex[lwork];
        memset(work, 0, sizeof(doublecomplex) * lwork);
        doublereal *rwork = new doublereal[lwork];
        memset(rwork, 0, sizeof(doublereal) * lwork);

        int index;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                index = (j + numRows * i);
                A[index].r = real(oMatrix(j, i));
                A[index].i = imag(oMatrix(j, i));
            }
        }

        char job = 'N';
        char jobR = 'V'; // compute the right eigenvectors
        zgeev_(&job, &jobR, &numRows, A, &numRows, eigVals, NULL, &numRows, vr, &numRows, work, &lwork, rwork, &info);

        ComplexMatrix *oResult = new ComplexMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                index = (j + numRows * i);
                Complex complexNr(
                        ls::RoundToTolerance(vr[index].r, gLapackTolerance),
                        ls::RoundToTolerance(vr[index].i, gLapackTolerance));

                (*oResult)(i, j) = complexNr;//.set(complex.Real, complex.Imag);
            }
        }

        delete[] eigVals;
        delete[] A;
        delete[] work;
        delete[] rwork;
        delete[] vr;

        return oResult;
    }

    void
    getSVD(DoubleMatrix &inputMatrix, DoubleMatrix *&outU, std::vector<double> *&outSingularVals, DoubleMatrix *&outV) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getSingularValsBySVD " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        integer numRows = inputMatrix.numRows();
        integer numCols = inputMatrix.numCols();

        integer min_MN = min(numRows, numCols);
        integer max_MN = max(numRows, numCols);

        if (min_MN == 0) return;

        //integer lwork    = 3*min_MN + max(max_MN, 7*min_MN);    // specified in dgesdd description
        integer lwork = 3 * min_MN * min_MN +
                        max(max_MN, 4 * min_MN * min_MN + 4 * min_MN); // specified in dgesdd description for job 'A'

        doublereal *A = inputMatrix.getCopy(true);
        doublereal *U = new doublereal[numRows * numRows];
        memset(U, 0, sizeof(doublereal) * numRows * numRows);
        doublereal *VT = new doublereal[numCols * numCols];
        memset(VT, 0, sizeof(doublereal) * numCols * numCols);
        doublereal *S = new doublereal[min_MN];
        memset(S, 0, sizeof(doublereal) * min_MN);
        doublereal *work = new doublereal[lwork];
        memset(work, 0, sizeof(doublereal) * lwork);
        integer *iwork = new integer[8 * min_MN];


        integer info;
        char jobz = 'A';
        dgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, U, &numRows, VT, &numCols, work, &lwork, iwork, &info);

        outU = new DoubleMatrix(numRows, numRows);
        int index;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                index = (j + numRows * i);
                (*outU)(j, i) = U[index];
            }
        }

        ls::RoundMatrixToTolerance(*outU, gLapackTolerance);

        outV = new DoubleMatrix(numCols, numCols);
        for (int i = 0; i < numCols; i++) {
            for (int j = 0; j < numCols; j++) {
                index = (j + numCols * i);
                (*outV)(i, j) = VT[index];
            }
        }

        ls::RoundMatrixToTolerance(*outV, gLapackTolerance);

        outSingularVals = new std::vector<double>();
        for (int i = 0; i < min_MN; i++) {
            outSingularVals->push_back(ls::RoundToTolerance(S[i], gLapackTolerance));
        }

        // free memory
        delete[] A;
        delete[] S;
        delete[] work;
        delete[] iwork;
        delete[] U;
        delete[] VT;
    }

    void ZgetSVD(ComplexMatrix &inputMatrix, ComplexMatrix *&outU, std::vector<double> *&outSingularVals,
                 ComplexMatrix *&outV) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getSingularValsBySVD " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        integer numRows = inputMatrix.numRows();
        integer numCols = inputMatrix.numCols();

        integer min_MN = min(numRows, numCols);
        integer max_MN = max(numRows, numCols);

        if (min_MN == 0) return;

        integer lwork = min_MN * min_MN + 2 * min_MN + max_MN; // specified in dgesdd description for job 'A'
        integer lrwork = 5 * min_MN * min_MN + 7 * min_MN;
        doublecomplex *A = new doublecomplex[numRows * numCols];
        memset(A, 0, sizeof(doublecomplex) * numRows * numCols);
        doublecomplex *U = new doublecomplex[numRows * numRows];
        memset(U, 0, sizeof(doublecomplex) * numRows * numRows);
        doublecomplex *VT = new doublecomplex[numCols * numCols];
        memset(VT, 0, sizeof(doublecomplex) * numCols * numCols);
        doublereal *S = new doublereal[min_MN];
        memset(S, 0, sizeof(doublereal) * min_MN);
        doublecomplex *work = new doublecomplex[lwork];
        memset(work, 0, sizeof(doublecomplex) * lwork);
        doublereal *rwork = new doublereal[lrwork];
        memset(rwork, 0, sizeof(doublereal) * lrwork);
        integer *iwork = new integer[8 * min_MN];

        int index;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                index = (j + numRows * i);
                A[index].r = real(inputMatrix(j, i));
                A[index].i = imag(inputMatrix(j, i));
            }
        }

        integer info;
        char jobz = 'A';
        zgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, U, &numRows, VT, &numCols, work, &lwork, rwork, iwork,
                &info);

        outU = new ComplexMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                index = (j + numRows * i);
                (*outU)(j, i) = Complex((ls::RoundToTolerance(U[index].r, gLapackTolerance),
                        ls::RoundToTolerance(U[index].i, gLapackTolerance)));
            }
        }

        outV = new ComplexMatrix(numCols, numCols);
        for (int i = 0; i < numCols; i++) {
            for (int j = 0; j < numCols; j++) {
                index = (j + numCols * i);
                (*outV)(i, j) = Complex(ls::RoundToTolerance(VT[index].r, gLapackTolerance),
                                        ls::RoundToTolerance(-VT[index].i, gLapackTolerance));
            }
        }

        outSingularVals = new std::vector<double>();
        for (int i = 0; i < min_MN; i++) {
            outSingularVals->push_back(ls::RoundToTolerance(S[i], gLapackTolerance));
        }

        // free memory
        delete[] A;
        delete[] S;
        delete[] work;
        delete[] iwork;
        delete[] U;
        delete[] VT;

        return;
    }

    double getRCond(DoubleMatrix &oMatrix) {
        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();

        integer minRC = min(numRows, numCols);

        if (minRC == 0) {
            return 0.0;
        }

        doublereal *A = (doublereal *) oMatrix.getCopy(true);
        integer *vecP = new integer[minRC];
        memset(vecP, 0, (sizeof(integer) * minRC));

        integer info;

        char norm = '1';
        integer order = numRows * numCols;

        double *work = new doublereal[4 * order];
        memset(work, 0, sizeof(double) * 4 * order);

        double dnorm = dlange_(&norm, &numRows, &numCols, A, &numRows, work);
        dgetrf_(&numRows, &numCols, A, &numRows, vecP, &info);
        ls::checkTolerance(numRows * numCols, A, gLapackTolerance);

        integer *iwork = new integer[numRows];
        memset(iwork, 0, sizeof(integer) * numRows);

        memset(work, 0, sizeof(double) * 4 * order);

        double rcond = 0.0;
        dgecon_(&norm, &numRows, A, &numRows, &dnorm, &rcond, work, iwork, &info);

        delete[] vecP;
        delete[] A;
        delete[] work;
        delete[] iwork;
        return rcond;
    }

    LU_Result *getLU(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getLU " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();

        int minRC = min(numRows, numCols);

        if (minRC == 0) {
            LU_Result *oResult = new LU_Result();
            DoubleMatrix *L = new DoubleMatrix(numRows, minRC);
            DoubleMatrix *U = new DoubleMatrix(minRC, numCols);
            IntMatrix *P = new IntMatrix(numRows, numRows);

            oResult->L = L;
            oResult->U = U;
            oResult->P = P;
            oResult->nInfo = -1;
            return oResult;
        }

        doublereal *A = (doublereal *) oMatrix.getCopy(true);
        integer *vecP = new integer[minRC];
        memset(vecP, 0, (sizeof(integer) * minRC));

        integer info;
        dgetrf_(&numRows, &numCols, A, &numRows, vecP, &info);

        ls::print(numRows, numCols, A);

        DoubleMatrix *L = new DoubleMatrix(numRows, minRC);
        DoubleMatrix *U = new DoubleMatrix(minRC, numCols);

        // Assign values to Lmat and Umat
        for (int i = 0; i < minRC; i++) {
            (*L)(i, i) = 1.0;
            (*U)(i, i) = A[i + numRows * i];
            for (int j = 0; j < i; j++) {
                (*L)(i, j) = A[i + numRows * j];
            }
            for (int j = i + 1; j < minRC; j++) {
                (*U)(i, j) = A[i + numRows * j];
            }
        }

        if (numRows > numCols) {
            for (int i = numCols; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    (*L)(i, j) = A[i + numRows * j];
                }
            }
        } else {
            for (int i = 0; i < numRows; i++) {
                for (int j = numRows; j < numCols; j++) {
                    (*U)(i, j) = A[i + numRows * j];
                }
            }
        }

        // build permutation matrix
        IntMatrix *P = new IntMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++)
            (*P)(i, i) = 1;
        for (int i = 0; i < minRC; i++) {
            if (vecP[i] != 0 && vecP[i] - 1 != i)
                P->swapRows(i, vecP[i] - 1);
        }

        LU_Result *oResult = new LU_Result();

        ls::RoundMatrixToTolerance(*L, gLapackTolerance);
        ls::RoundMatrixToTolerance(*U, gLapackTolerance);

        oResult->L = L;
        oResult->U = U;
        oResult->P = P;
        oResult->nInfo = info;

        delete[] A;
        delete[] vecP;

        return oResult;
    }

    LU_Result *getLUwithFullPivoting(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== getLUwithFullPivoting " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();

        if (numRows != numCols)
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");


        doublereal *A = (doublereal *) oMatrix.getCopy(true);
        integer *vecP = new integer[numRows];
        memset(vecP, 0, (sizeof(integer) * numRows));
        integer *vecQ = new integer[numRows];
        memset(vecQ, 0, (sizeof(integer) * numRows));

        integer info;
        dgetc2_(&numRows, A, &numRows, vecP, vecQ, &info);

        DoubleMatrix *L = new DoubleMatrix(numRows, numRows);
        DoubleMatrix *U = new DoubleMatrix(numRows, numCols);

        // Assign values to Lmat and Umat
        for (int i = 0; i < numRows; i++) {
            (*L)(i, i) = 1.0;
            (*U)(i, i) = A[i + numRows * i];
            for (int j = 0; j < i; j++) {
                (*L)(i, j) = A[i + numRows * j];
            }
            for (int j = i + 1; j < numRows; j++) {
                (*U)(i, j) = A[i + numRows * j];
            }
        }

        if (numRows > numCols) {
            for (int i = numCols; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    (*L)(i, j) = A[i + numRows * j];
                }
            }
        } else {
            for (int i = 0; i < numRows; i++) {
                for (int j = numRows; j < numCols; j++) {
                    (*U)(i, j) = A[i + numRows * j];
                }
            }
        }

        // build permutation matrix
        IntMatrix *P = new IntMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++)
            (*P)(i, i) = 1;
        for (int i = 0; i < numRows; i++) {
            if (vecP[i] != 0 && vecP[i] - 1 != i)
                P->swapRows(i, vecP[i] - 1);
        }

        IntMatrix *Q = new IntMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++)
            (*Q)(i, i) = 1;
        for (int i = 0; i < numRows; i++) {
            if (vecQ[i] != 0 && vecQ[i] - 1 != i)
                Q->swapCols(i, vecQ[i] - 1);
        }

        LU_Result *oResult = new LU_Result();

        ls::RoundMatrixToTolerance(*L, gLapackTolerance);
        ls::RoundMatrixToTolerance(*U, gLapackTolerance);

        oResult->L = L;
        oResult->U = U;
        oResult->P = P;
        oResult->Q = Q;
        oResult->nInfo = info;

        delete[] A;
        delete[] vecP;
        delete[] vecQ;

        return oResult;
    }

    DoubleMatrix *inverse(DoubleMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== inverse " << endl;
        //Log(lDebug5) << "======================================================" << endl << endl;
        DoubleMatrix *oResultMatrix = NULL;

        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();


        if (numRows != numCols)
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");


        doublereal *A = oMatrix.getCopy(true);
        integer *ipvt = new integer[numRows];
        memset(ipvt, 0, sizeof(integer) * numRows);
        doublereal *work = new doublereal[numRows];
        memset(work, 0, sizeof(doublereal) * numRows);

        //Log(lDebug5) << "Input Matrix 1D: \n"<<ls::print(numRows, numRows, A);


        // Carry out LU Factorization
        integer info;
        dgetrf_(&numRows, &numRows, A, &numRows, ipvt, &info);
        if (info < 0)
            throw ApplicationException("Error in dgetrf : LU Factorization", "Illegal Value");
        if (info > 0)
            throw ApplicationException("Exception in ls while computing Inverse", "Input Matrix is Singular.");


        //Log(lDebug5) << "After dgetrf: \n"<<ls::print(numRows, numRows, A);

        dgetri_(&numRows, A, &numRows, ipvt, work, &numRows, &info);

        //Log(lDebug5) << "After dgetri: \n"<<ls::print(numRows, numRows, A);

        oResultMatrix = new DoubleMatrix(A, numRows, numRows, true);
        ls::RoundMatrixToTolerance(*oResultMatrix, gLapackTolerance);

        delete[] A;
        delete[] ipvt;
        delete[] work;

        return oResultMatrix;
    }

    ComplexMatrix *Zinverse(ComplexMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== Zinverse " << endl;
        //Log(lDebug5) << "======================================================" << endl;

        ComplexMatrix *oResultMatrix = NULL;
        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();

        if (numRows != numCols) {
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");
        }

        doublecomplex *A = new doublecomplex[numRows * numRows];
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                A[i + numRows * j].r = real(oMatrix(i, j));
                A[i + numRows * j].i = imag(oMatrix(i, j));
            }
        }

        ////Log(lDebug5) << "Input Matrix 1D: \n";
        //ls::print(numRows, numRows, A);

        integer *ipvt = new integer[numRows];
        memset(ipvt, 0, sizeof(integer) * numRows);
        doublecomplex *work = new doublecomplex[numRows];
        memset(work, 0, sizeof(doublecomplex) * numRows);

        // Carry out LU Factorization
        integer info;
        zgetrf_(&numRows, &numRows, A, &numRows, ipvt, &info);

        if (info < 0) {
            throw ApplicationException("Error in dgetrf : LU Factorization", "Illegal Value");
        }

        if (info > 0) {
            throw ApplicationException("Exception in ls while computing Inverse", "Input Matrix is Sinuglar.");
        }

        ////Log(lDebug5) << "After dgetrf: \n";
        //ls::print(numRows, numRows, A);

        zgetri_(&numRows, A, &numRows, ipvt, work, &numRows, &info);

        //Log(lDebug5) << "After dgetri: \n";
        //ls::print(numRows, numRows, A);

        oResultMatrix = new ComplexMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                Complex tols(ls::RoundToTolerance(A[(i + numRows * j)].r, gLapackTolerance),
                             ls::RoundToTolerance(A[(i + numRows * j)].i, gLapackTolerance));
                (*oResultMatrix)(i, j) = tols;

            }
        }

        // free memory
        delete[] A;
        delete[] ipvt;
        delete[] work;

        return oResultMatrix;
    }

    ComplexMatrix *Zinverse(const ComplexMatrix &oMatrix) {
        //Log(lDebug5) << "======================================================" << endl;
        //Log(lDebug5) << "=== Zinverse " << endl;
        //Log(lDebug5) << "======================================================" << endl;

        ComplexMatrix *oResultMatrix = NULL;
        integer numRows = oMatrix.numRows();
        integer numCols = oMatrix.numCols();

        if (numRows != numCols) {
            throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");
        }

        doublecomplex *A = new doublecomplex[numRows * numRows];
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                A[i + numRows * j].r = real(oMatrix(i, j));
                A[i + numRows * j].i = imag(oMatrix(i, j));
            }
        }

        ////Log(lDebug5) << "Input Matrix 1D: \n";
        //ls::print(numRows, numRows, A);

        integer *ipvt = new integer[numRows];
        memset(ipvt, 0, sizeof(integer) * numRows);
        doublecomplex *work = new doublecomplex[numRows];
        memset(work, 0, sizeof(doublecomplex) * numRows);

        // Carry out LU Factorization
        integer info;
        zgetrf_(&numRows, &numRows, A, &numRows, ipvt, &info);

        if (info < 0) {
            throw ApplicationException("Error in dgetrf : LU Factorization", "Illegal Value");
        }

        if (info > 0) {
            throw ApplicationException("Exception in ls while computing Inverse", "Input Matrix is Sinuglar.");
        }

        ////Log(lDebug5) << "After dgetrf: \n";
        //ls::print(numRows, numRows, A);

        zgetri_(&numRows, A, &numRows, ipvt, work, &numRows, &info);

        //Log(lDebug5) << "After dgetri: \n";
        //ls::print(numRows, numRows, A);

        oResultMatrix = new ComplexMatrix(numRows, numRows);
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numRows; j++) {
                Complex tols(ls::RoundToTolerance(A[(i + numRows * j)].r, gLapackTolerance),
                             ls::RoundToTolerance(A[(i + numRows * j)].i, gLapackTolerance));
                (*oResultMatrix)(i, j) = tols;
            }
        }

        // free memory
        delete[] A;
        delete[] ipvt;
        delete[] work;
        return oResultMatrix;
    }



//LibLA* getInstance()
//{
//    if (_Instance == NULL)
//        _Instance = new LibLA();
//    return _Instance;
//}


// static variable definitions
//LibLA* _Instance = NULL;


} //Namespace ls
