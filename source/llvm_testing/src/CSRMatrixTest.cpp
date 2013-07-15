/*
 * CSRMatrixTest.cpp
 *
 *  Created on: Jul 11, 2013
 *      Author: andy
 */

#include "CSRMatrixTest.h"

#include "tests.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMModelDataSymbols.h"
#include "rrLLVMModelDataIRBuilder.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrLLVMIncludes.h"
#include "rrLLVMAssignmentRuleEvaluator.h"
#include "rrLLVMEvalInitialConditionsCodeGen.h"
#include "rrSparse.h"





#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SBMLReader.h>

#include "rrModelData.h"

#include <utility>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "cpplapack.h"

using namespace std;
using namespace rr;
using namespace llvm;
using namespace libsbml;


bool runSparseTest(const int m, const int n, const int nnz)
{
    double *A = (double*)calloc(m*n, sizeof(double));

    double *x = (double*)calloc(n, sizeof(double));

    double *y = (double*)calloc(m, sizeof(double));
    double *ys = (double*)calloc(m, sizeof(double));

    int true_nnz = 0;


    // row major:    offset = row*NUMCOLS + column
    // column major: offset = row + column*NUMROWS


    // set x and first row of A to ones.
    //for (int i = 0; i < n; i++) {
    //    A[0 + i*m] = 1;
    //    x[i] = 1;
    //}

    for (int k = 0; k < nnz; k++) {
        int i = rand() % m;
        int j = rand() % n;

        if (A[i + j*m] == 0) {
            A[i + j*m] = 1;
            true_nnz++;
        }
    }

    for (int k = 0; k < n; k++) {
        x[k] = rand() % 10;
    }

    //cout << "A: ";
    //for(int i = 0; i < m*n; i++) {
    //    cout << A[i] << ",";
    //}
    //cout << "\n";

    cout << "x: ";
    for(int i = 0; i < n; i++) {
        cout << x[i] << ",";
    }
    cout << "\n";


    cpp_dgemv('n', m, n, 1, A, m, x, 1, 0, y, 1);

    cout << "dense y:  ";
    for(int i = 0; i < m; i++) {
        cout << y[i] << ",";
    }
    cout << "\n";


    vector<int> rowidx;
    vector<int> colidx;
    vector<double> values;

    for(int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double value = A[i + j*m];

            if (value != 0)
            {
                rowidx.push_back(i);
                colidx.push_back(j);
                values.push_back(value);
            }
        }
    }

    assert(rowidx.size() == true_nnz);
    assert(colidx.size() == true_nnz);
    assert(values.size() == true_nnz);

    csr_matrix *mat = csr_matrix_new(m, n, rowidx, colidx, values);

    csr_matrix_dgemv(mat, x, ys);

    cout << "sparse y: ";
    for(int i = 0; i < m; i++) {
        cout << ys[i] << ",";
    }
    cout << "\n";

    // change some values
    for(int k = 0; k < true_nnz; k++)
    {
        int i = rowidx[k];
        int j = colidx[k];
        double value = rand() % 10;

        csr_matrix_set_nz(mat, i, j, value);
        A[i + j*m] = value;

        assert(csr_matrix_get_nz(mat, i, j) == value);
    }

    memset(y, 0, m*sizeof(double));
    cpp_dgemv('n', m, n, 1, A, m, x, 1, 0, y, 1);

    cout << "dense y:  ";
    for (int i = 0; i < m; i++)
    {
        cout << y[i] << ",";
    }
    cout << "\n";


    memset(ys, 0, m*sizeof(double));
    csr_matrix_dgemv(mat, x, ys);

    cout << "sparse y: ";
    for (int i = 0; i < m; i++)
    {
        cout << ys[i] << ",";
    }
    cout << "\n";

    csr_matrix_delete(mat);


    free(A);
    free(x);
    free(y);
    free(ys);
}
