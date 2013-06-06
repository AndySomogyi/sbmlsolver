#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "UnitTest++.h"
#include "rrc_api.h"
#include "rrUtils.h"
#include "rrIniFile.h"
#include "rrException.h"
#include "src/TestUtils.h"

using namespace UnitTest;
using namespace std;
using namespace rr;
using namespace rrc;

extern string   gTempFolder;
extern string   gTestDataFolder;
extern string   gRRInstallFolder;
extern bool     gDebug;

SUITE(LIBSTRUCT_TESTS)
{
string TestDataFileName     = "LibStructTest.dat";
IniFile iniFile;
string TestModelFileName;
RRHandle gRR = NULL;

    TEST(LS_TEST_DATA_FILES)
    {
    	string sec("LS_TESTS");
        string key("InputFile");

        gRR                         = createRRInstanceEx(gTempFolder.c_str());
        string testDataFileName     = joinPath(gTestDataFolder, TestDataFileName);
        CHECK(fileExists(testDataFileName));
        CHECK(iniFile.Load(testDataFileName));

        clog<<"Loaded test data from file: "<< testDataFileName;
        if(iniFile.GetSection(sec))
        {
            IniSection* sbml = iniFile.GetSection(sec);
            IniKey* fNameKey = sbml->GetKey(key);
            if(fNameKey)
            {
                TestModelFileName  = joinPath(gTestDataFolder, fNameKey->mValue);
                CHECK(fileExists(TestModelFileName));
            }
        }
        CHECK(loadSBMLFromFileE(gRR, TestModelFileName.c_str(), true));
    }

    TEST(GET_L0_MATRIX)
    {
    	string section("L0_MATRIX");

        if(iniFile.GetSection(section))
        {
            IniSection* 	  isec		= iniFile.GetSection(section);
            ls::DoubleMatrix  ref 		= getDoubleMatrixFromString(isec->GetNonKeysAsString());
            RRMatrix* 		  actual 	= getLinkMatrix(gRR);

            //Check dimensions
            if(actual->RSize != ref.RSize() || actual->CSize != ref.CSize())
            {
                CHECK(false);
                return;
            }

            for(int row = 0; row < actual->RSize; row++)
            {
                for(int col = 0; col < actual->CSize; col++)
                {
                  CHECK_CLOSE(ref(row,col), actual->Data[row*actual->CSize + col], 1e-6);
                }
            }
        }
    }

    TEST(GET_EIGEN_VECTORS)
    {
    	string section("EIGEN_VECTOR_MATRIX");

        if(iniFile.GetSection(section))
        {
            IniSection* 	  isec		= iniFile.GetSection(section);
            ls::ComplexMatrix ref 		= getComplexMatrixFromString(isec->GetNonKeysAsString());
            RRMatrix* 		  input 	= getLinkMatrix(gRR);
            RRComplexMatrix*  actual 	= getEigenVectors(input);

            char* mat = complexMatrixToString(actual);
            clog<<mat;
            //Check dimensions
            if(actual->RSize != ref.RSize() || actual->CSize != ref.CSize())
            {
                CHECK(false);
                return;
            }

            for(int row = 0; row < actual->RSize; row++)
            {
                for(int col = 0; col < actual->CSize; col++)
                {
                	CHECK_CLOSE(ref(row,col).Real, actual->Data[row*actual->CSize + col].re, 1e-6);
                	CHECK_CLOSE(ref(row,col).Imag, actual->Data[row*actual->CSize + col].imag, 1e-6);
                }
            }
        }
    }

}





