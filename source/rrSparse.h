/*
 * rrCSparse.h
 *
 *  Created on: Jul 10, 2013
 *      Author: andy
 */

#ifndef RRCSPARSE_H_
#define RRCSPARSE_H_

#include "rrModelData.h"
#include "rrOSSpecifics.h"
#include <vector>
#include <ostream>

namespace rr
{


/**
 * allocate and initialize the buffers and fields of a dcsr_matrix
 * struct.
 *
 * TODO: These belong someplace more appropriate that this header file.
 *
 * These funcs are used by the generated LLVM model.
 */
csr_matrix* RR_DECLSPEC csr_matrix_new(int m, int n,
        const std::vector<int>& rowidx, const std::vector<int>& colidx,
        const std::vector<double>& values);

/**
 * free the buffers belonging to a dcsr_matrix, and the
 * matrix itself.
 */
void RR_DECLSPEC csr_matrix_delete(csr_matrix *mat);

/**
 * sets a (previously allocted) non-zero value to the given value.
 *
 * @returns true on success, false on failure.
 */
bool RR_DECLSPEC csr_matrix_set_nz(csr_matrix *mat, int row, int col, double val);

/**
 * sets a (previously allocted) non-zero value to the given value.
 *
 * returns NaN if the entry (i,j) has not been alocated.
 */
double RR_DECLSPEC csr_matrix_get_nz(const csr_matrix *mat, int row, int col);

/**
 * peform a matrix vector multiply y = Ax + y
 *
 * The given vectors y and x must be the same size as number of
 * columns in the sparse matrix.
 */
void RR_DECLSPEC csr_matrix_dgemv(const csr_matrix *A, double const *x, double *y);

/**
 * dump the matrix to an output stream.
 */
std::ostream& RR_DECLSPEC operator<< (std::ostream& os, const csr_matrix* mat);


}

#endif /* RRCSPARSE_H_ */
