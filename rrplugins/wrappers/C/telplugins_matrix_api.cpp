#pragma hdrstop
#include "rr-libstruct/lsMatrix.h"
#include "rrplugins/common/telOSSpecifics.h"
#include "telplugins_matrix_api.h"
#include "tel_macros.h"
#include "telplugins_utilities.h"
#include "telplugins_cpp_support.h"
#include "rrplugins/common/telStringUtils.h"
#include "rrplugins/common/telTelluriumData.h"
//#include "rrc_macros.h"

using namespace std;
using tlp::StringList;

typedef ls::Matrix<double> Matrix;




double* tlp_cc tpGetDataArray(TELHandle handle)
{
    start_try
        Matrix* matrixH = castHandle< ls::Matrix<double> >(handle, __FILE__);
        return matrixH->getArray();
    tel_catch_ptr_macro
}

int tlp_cc tpGetMatrixNumRows(TELHandle handle)
{
    start_try
        Matrix* matrixH = castHandle< ls::Matrix<double> >(handle, __FILE__);
        return matrixH->RSize();
    tel_catch_int_macro
}

int tlp_cc tpGetMatrixNumCols(TELHandle handle)
{
    start_try
        Matrix* matrixH = castHandle< ls::Matrix<double> >(handle, __FILE__);
        return matrixH->CSize();
    tel_catch_int_macro
}

bool tlp_cc tpGetMatrixElement(TELHandle handle, int row, int col, double* value)
{
    start_try
        Matrix* matrixH = castHandle< ls::Matrix<double> >(handle, __FILE__);
        *value = matrixH->Element(row, col);
        return true;
    tel_catch_bool_macro
}

bool tlp_cc tpSetMatrixElement(TELHandle handle, int row, int col, double value)
{
    start_try
        Matrix* matrix = castHandle< ls::Matrix<double> >(handle, __FUNC__);
        (*matrix)(row, col) =  value;
        return true;
    tel_catch_bool_macro
}

TELHandle tlp_cc tpCreateMatrix(int nRows, int nCols, char* colNames)
{
    start_try
        Matrix* data = new Matrix(nRows, nCols);
        gHM.registerHandle(data, typeid(data).name());
        if (colNames)
        {
            string cNames(colNames);
        }
        return data;
    tel_catch_ptr_macro
}

bool tlp_cc tpFreeMatrix(TELHandle handle)
{
    start_try
        Matrix* data = castHandle< Matrix >(handle, __FUNC__);
        delete data;
        return true;
    tel_catch_bool_macro
}
