#include <vector>
#include "rrUtils.h"
#include "TestUtils.h"

using namespace std;
using namespace rr;
using namespace ls;

vector< complex<double> > getComplexVectorFromString(const string& text)
{
	vector< complex<double> > vec;

    //Parse the vector
    vector<string> rows = splitString(text, "\n");
    for(int row = 0; row < rows.size(); row++)
    {
		vec.push_back(toComplex(rows[row]));
    }
	return vec;
}

ls::DoubleMatrix getDoubleMatrixFromString(const string& textMatrix)
{
	DoubleMatrix mat;

    //Parse the matrix
    vector<string> rows = splitString(textMatrix, "\n");
    for(int row = 0; row < rows.size(); row++)
    {
        vector<string> values = splitString(rows[row], " \t");
        for(int col = 0; col < values.size(); col++)
        {
        	if(!mat.size())
            {
                mat.resize(rows.size(), values.size());
            }

            mat(row, col) = toDouble(values[col]);
        }
    }
	return mat;
}

ls::ComplexMatrix getComplexMatrixFromString(const string& textMatrix)
{
	ComplexMatrix mat;

    //Parse the matrix
    vector<string> rows = splitString(textMatrix, "\n");
    for(int row = 0; row < rows.size(); row++)
    {
        vector<string> values = splitString(rows[row], " \t");
        for(int col = 0; col < values.size(); col++)
        {
        	if(!mat.size())
            {
                mat.resize(rows.size(), values.size());
            }

            std::complex<double> val = toComplex(values[col]);
            mat(row, col).Real = real(val);
			mat(row, col).Imag = imag(val);
        }
    }
	return mat;
}

