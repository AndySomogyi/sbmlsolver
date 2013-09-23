#ifndef TestUtilsH
#define TestUtilsH
#include <string>
#include "lsMatrix.h"


std::vector< std::complex<double> > getComplexVectorFromString(const std::string& text);
ls::DoubleMatrix getDoubleMatrixFromString(const std::string& textMatrix);
ls::ComplexMatrix getComplexMatrixFromString(const std::string& textMatrix);


#endif
