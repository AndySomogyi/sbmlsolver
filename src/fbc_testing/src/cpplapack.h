/*
 * cpplapack.h
 *
 *  Created on: Jul 10, 2013
 *      Author: andy
 */

#ifndef CPPLAPACK_H_
#define CPPLAPACK_H_

int cpp_dgemv(char trans, int m, int n, double alpha, double *a, int lda,
        double *x, int incx, double beta, double *y, int incy);

#endif /* CPPLAPACK_H_ */
