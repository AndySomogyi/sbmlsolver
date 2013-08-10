/*
 * LLVMCSRMatrixTest.h
 *
 *  Created on: Jul 11, 2013
 *      Author: andy
 */

#ifndef LLVMCSRMATRIXTEST_H_
#define LLVMCSRMATRIXTEST_H_

#include "rrSparse.h"
#include "llvm/LLVMIncludes.h"
#include "llvm/ModelGeneratorContext.h"

namespace rr
{

typedef bool (*CsrMatrixSetNZPtr)(csr_matrix *mat, int row, int col, double val);

typedef double (*CsrMatrixGetNZPtr)(csr_matrix *mat, int row, int col);

class LLVMCSRMatrixTest
{
public:
    LLVMCSRMatrixTest();
    virtual ~LLVMCSRMatrixTest();

    bool callCSRMatrixSetNZ(csr_matrix *mat, int row, int col, double value);

    double callCSRMatrixGetNZ(csr_matrix *mat, int row, int col);

    llvm::Function *getCSRMatrixSetNZTestFunc();

    llvm::Function *getCSRMatrixGetNZTestFunc();


private:
    llvm::Function *setFunc;
    llvm::Function *getFunc;

    ModelGeneratorContext context;
};


bool runLLVMCSRMatrixTest(const int m, const int n, const int nnz);

} /* namespace rr */
#endif /* LLVMCSRMATRIXTEST_H_ */
