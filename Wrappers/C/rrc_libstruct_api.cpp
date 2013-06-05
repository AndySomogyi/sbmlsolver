#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rr-libstruct/lsLibLA.h"
#include "rrc_libstruct_api.h"
#include "rrc_cpp_support.h"
#include "rrc_macros.h"
//---------------------------------------------------------------------------
namespace rrc
{
using namespace rr;
//using namespace ls;	//Libstruct namespace
//LibStruct forwarded functions

RRComplexMatrixHandle rrcCallConv getEigenVectors(const RRMatrixHandle inMatrix)
{
    try
    {
    	DoubleMatrix* mat =  createMatrix(inMatrix) ;
        if(mat)
        {
        	ComplexMatrix* cMat  = ls::getEigenVectors((*mat));
            return createMatrix(cMat);
        }
        else
        {
        	return NULL;
        }
    }
    catch_ptr_macro

}

} //namespace rrc
