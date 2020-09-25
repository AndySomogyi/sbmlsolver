#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "unit_test/UnitTest++.h"
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
extern string gCompiler;

SUITE(LIBSTRUCT_TESTS)
{
string TestDataFileName     = "LibStructTest.dat";
IniFile iniFile;
string TestModelFileName;
RRHandle gRR = NULL;

    TEST(testLibStructTestsDataFiles)
    {
        string sec("LS_TESTS");
        string key("InputFile");

        gRR                         = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        string testDataFileName     = joinPath(gTestDataFolder, TestDataFileName);
        CHECK(fileExists(testDataFileName));
        CHECK(iniFile.Load(testDataFileName));

        //clog<<"Loaded test data from file: "<< testDataFileName;
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

    TEST(getLinkMatrix)
    {
        string section("getLinkMatrix");
        if(iniFile.GetSection(section))
        {
            IniSection*       isec        = iniFile.GetSection(section);
            ls::DoubleMatrix  ref         = getDoubleMatrixFromString(isec->GetNonKeysAsString());

            //Testing call
            RRDoubleMatrix*           actual     = getLinkMatrix(gRR);

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

    TEST(getEigenValuesMatrix)
    {
        string section("getEigenValuesMatrix");

        if(iniFile.GetSection(section))
        {
            IniSection*         isec    = iniFile.GetSection(section);
            ls::DoubleMatrix      ref     = getDoubleMatrixFromString(isec->GetNonKeysAsString());
            RRDoubleMatrix*             input     = getLinkMatrix(gRR);

            //Testing call
            RRDoubleMatrix*                actual     = getEigenvaluesMatrix(input);
            /////////////////////////////////

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

    TEST(getEigenValuesVector)
    {
        string section("getEigenvaluesVector");

        if(iniFile.GetSection(section))
        {
            IniSection*                   isec      = iniFile.GetSection(section);
            vector< complex<double> >     ref       = getComplexVectorFromString(isec->GetNonKeysAsString());
            RRDoubleMatrix*                  input     = getLinkMatrix(gRR);
            //Testing call
            RRComplexVector*              actual     = getEigenvaluesVector(input);
            //////////////////////////////////////////////////////////////////

            //Check dimensions
            if(actual->Count != ref.size())
            {
                CHECK(false);
                return;
            }

            for(int row = 0; row < actual->Count; row++)
            {
                    CHECK_CLOSE(real(ref[row]), actual->Data[row].re, 1e-6);
                    CHECK_CLOSE(imag(ref[row]), actual->Data[row].imag, 1e-6);
            }
        }
    }

    TEST(FREE_RR_INSTANCE)
    {
        CHECK(freeRRInstance(gRR));
        gRR = NULL;
    }

}
