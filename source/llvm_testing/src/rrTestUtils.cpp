#include <vector>
#include "rrUtils.h"
#include "rrTestUtils.h"


using namespace rr;
DoubleMatrix ParseMatrixFromText(const std::string& textMatrix)
{
	DoubleMatrix mat;

    //Parse the matrix
    std::vector<std::string> rows = splitString(textMatrix, "\n");
    for(int row = 0; row < rows.size(); row++)
    {
        std::vector<std::string> values = splitString(rows[row], " \t");
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

