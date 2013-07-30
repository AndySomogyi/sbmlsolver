/*
 * rrCSparse.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: andy
 */

#include "rrSparse.h"
#include "rrLogger.h"
#include "rrExecutableModel.h"
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <set>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <cmath>
#include <cstdio>

namespace rr {

using namespace std;

// sorting predicate for vector of column values,
// sort on the index, take value along for the ride.
struct sort_pred
{
    bool operator()(const std::pair<int,double> &left, const std::pair<int,double> &right)
    {
        return left.first < right.first;
    }
};

csr_matrix* csr_matrix_new(int m, int n,
        const std::vector<int>& rowidx, const std::vector<int>& colidx,
        const std::vector<double>& values)
{
    char err[64];
    int nnz = rowidx.size();

    if (colidx.size() != nnz || values.size() != nnz)
    {
        throw runtime_error("rowidx, colidx and values must be the same length");
    }

    for (int i = 0; i < nnz; i++)
    {
        if (rowidx[i] >= m)
        {
            snprintf(err, sizeof(err)/sizeof(char),
                    "rowidx[%i] == %i >= row count %i", i, rowidx[i], m);
            throw runtime_error(err);
        }
        if (colidx[i] >= n)
        {
            snprintf(err, sizeof(err)/sizeof(char),
                    "colidx[%i] == %i >= column count %i", i, colidx[i], n);
            throw runtime_error(err);
        }
    }

    csr_matrix* mat = (csr_matrix*)calloc(1, sizeof(csr_matrix));

    // values that will get stuffed into struct
    vector<double> mvalues;
    vector<int> mcolidx;
    vector<int> mrowptr;

    mat->m = m;
    mat->n = n;
    mat->nnz = 0;

    // first index
    mrowptr.push_back(0);

    for(int i = 0; i < m; i++)
    {
        // find all the values in this row
        vector<pair<int,double> > cols;
        for(int j = 0; j < nnz; j++)
        {
            if (i == rowidx[j])
            {
                cols.push_back(pair<int,double>(colidx[j], values[j]));
            }
        }

        // sort by the column index
        sort(cols.begin(), cols.end(), sort_pred());

        for(vector<pair<int,double> >::const_iterator j = cols.begin(); j != cols.end(); j++)
        {
            mcolidx.push_back(j->first);
            mvalues.push_back(j->second);
        }
        mrowptr.push_back((mat->nnz += cols.size()));
    }

    assert(mat->nnz == nnz);
    assert(mcolidx.size() == nnz);
    assert(mvalues.size() == nnz);
    assert(mrowptr.size() == mat->m + 1);

    mat->rowptr = (int*)calloc(mat->m + 1, sizeof(int));
    mat->colidx = (int*)calloc(nnz, sizeof(int));
    mat->values = (double*)calloc(nnz, sizeof(double));

    memcpy(mat->rowptr, &mrowptr[0], (mat->m+1)*sizeof(int));
    memcpy(mat->colidx, &mcolidx[0], nnz*sizeof(int));
    memcpy(mat->values, &mvalues[0], nnz*sizeof(double));

    return mat;
}

bool csr_matrix_set_nz(csr_matrix* mat, int row, int col, double val)
{
    if (mat && row <= mat->m && col <= mat->n)
    {
        for (int k = mat->rowptr[row]; k < mat->rowptr[row + 1]; k++)
        {
            if (mat->colidx[k] == col)
            {
                mat->values[k] = val;
                return true;
            }
        }
    }
    return false;
}

double csr_matrix_get_nz(const csr_matrix* mat, int row, int col)
{
    if (mat && row <= mat->m && col <= mat->n)
    {
        for (int k = mat->rowptr[row]; k < mat->rowptr[row + 1]; k++)
        {
            if (mat->colidx[k] == col)
            {
                return mat->values[k];
            }
        }
    }
    return nan("dcsr_matrix_get_nz not found");
}

void csr_matrix_delete(csr_matrix* mat)
{
    if (mat)
    {
        free(mat->colidx);
        free(mat->rowptr);
        free(mat->values);
        free(mat);
    }
}

void csr_matrix_dgemv(const csr_matrix* A, const double* x, double* y)
{
    const int m = A->m;
    int *rowptr = A->rowptr;
    int *colidx = A->colidx;
    double *values = A->values;
    for (int i = 0; i < m; i++)
    {
        double yi = y[i];
        for (int k = rowptr[i]; k < rowptr[i + 1]; k++)
        {
            yi = yi + values[k] * x[colidx[k]];
        }
        y[i] = yi;
    }
}



std::ostream& operator <<(std::ostream& os, const csr_matrix* mat)
{
    os.precision(2);                   // Set 2 digits past the decimal
    os.flags(ios::right | ios::fixed); // Fixed point, right justified

    os << "csr_matrix\n";

    if (mat == 0)
    {
        os << "NULL" << endl;
        return os;
    }

    os << "rows: " << mat->m << ", columns: " << mat->n;
    os << ", non-zero entries: " << mat->nnz << endl;
    if (mat->nnz > 0)
    {
        os << '[';
        for (int m = 0; m < mat->m; ++m)
        {
            if (m != 0)
                os << ' ';
            os << '[';
            for (int n = 0; n < mat->n; ++n)
            {
                double val = csr_matrix_get_nz(mat, m, n);
                os.width(7);
                os << (std::isnan(val) ? 0 : val);
                if (n < mat->n - 1)
                {
                    os << ", ";
                }
                else
                {
                    os << ']';
                }
            }
            if (m < mat->m - 1)
            {
                os << endl;
            }
            else
            {
                os << ']' << endl;
            }
        }
    }
    else
    {
        os << "[[]]" << endl;
    }

    return os;
}

}


