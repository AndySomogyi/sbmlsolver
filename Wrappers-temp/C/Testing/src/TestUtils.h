#ifndef TestUtilsH
#define TestUtilsH
#include <string>
#include "lsMatrix.h"

//using namespace ls;
using std::string;
vector< complex<double> > getComplexVectorFromString(const string& text);
ls::DoubleMatrix getDoubleMatrixFromString(const string& textMatrix);
ls::ComplexMatrix getComplexMatrixFromString(const string& textMatrix);


#endif
