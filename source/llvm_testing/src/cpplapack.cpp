/*
 * cpplapack.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "cpplapack.h"

extern "C"
{
#include "clapack/f2c.h"
#include "clapack/clapack.h"
}

int cpp_dgemv(char trans, int m, int n, double alpha, double* a, int lda,
        double* x, int incx, double beta, double* y, int incy)
{
    integer _m = m;
    integer _n = n;
    integer _lda = lda;
    integer _incx = incx;
    integer _incy = incy;
    return dgemv_(&trans, &_m, &_n, &alpha, a, &_lda, x, &_incx, &beta, y, &_incy);
}
