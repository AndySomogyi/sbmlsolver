#include "gtest/gtest.h"
#include "test_util.h"
#include "sbml/SBMLTypes.h"
#include <vector>
#include "rrUtils.h"

using namespace std;
using namespace rr;
using namespace ls;

std::string getModelDir(std::string base, std::string suite, std::string name)
{
	return base + "models/" + suite + "/" + name + "/";
}

bool testValidateSBML(std::string sbml)
{
	libsbml::SBMLDocument* doc = libsbml::readSBMLFromString(sbml.c_str());
	bool result = true;

	doc->setConsistencyChecks(libsbml::LIBSBML_CAT_MODELING_PRACTICE, false);
	doc->setConsistencyChecks(libsbml::LIBSBML_CAT_UNITS_CONSISTENCY, false);
	doc->checkConsistency();
	libsbml::SBMLErrorLog* log = doc->getErrorLog();
	for (int i = 0; i < log->getNumFailsWithSeverity(2); i++)
	{
		std::cout << log->getErrorWithSeverity(i, 2)->getMessage() << std::endl;
		result = false;
	}
	delete doc;
	return result;
}

DoubleMatrix ParseMatrixFromText(const string& textMatrix)
{
	DoubleMatrix mat;

	//Parse the matrix
	vector<string> rows = splitString(textMatrix, "\n");
	for (int row = 0; row < rows.size(); row++)
	{
		vector<string> values = splitString(rows[row], " \t");
		for (int col = 0; col < values.size(); col++)
		{
			if (!mat.size())
			{
				mat.resize(static_cast<unsigned int>(rows.size()), static_cast<unsigned int>(values.size()));
			}

			mat(row, col) = toDouble(values[col]);
		}
	}
	return mat;
}

void expectArrayClose(const ls::DoubleMatrix& jRef, const ls::DoubleMatrix& jFull, double epsilon)
{
    ASSERT_EQ(jRef.RSize(), jFull.RSize());
	ASSERT_EQ(jRef.CSize(), jFull.CSize());
	for (unsigned int r = 0; r < jRef.RSize(); r++) {
		for (unsigned int c = 0; c < jRef.CSize(); c++) {
			EXPECT_NEAR(jRef.Element(r, c), jFull.Element(r, c), epsilon);
		}
	}
}

