#include <vector>
#include "rrUtils.h"
#include "rrTestUtils.h"

using namespace std;
using namespace rr;
DoubleMatrix ParseMatrixFromText(const string& textMatrix)
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

