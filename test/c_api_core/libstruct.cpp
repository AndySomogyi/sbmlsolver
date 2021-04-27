#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "gtest/gtest.h"
#include "C/rrc_api.h"
#include "rrUtils.h"
#include "rrIniFile.h"
#include "rrException.h"
#include "../test_util.h"
#include <filesystem>
#include "RoadRunnerTest.h"
#include "CAPICoreTest.h"

using namespace std;
using namespace rr;
using namespace rrc;
using std::filesystem::path;


class LibstructTests : public CAPICoreTest {
public:
    path LS_TestDataFileName;
    path LS_TestModelFileName;
    IniFile iniFile;

    LibstructTests() {
        LS_TestDataFileName  = cAPICoreModelsDir / "LibStructTest.dat";
        if (!fs::exists(LS_TestDataFileName)){
            throw std::invalid_argument("LibstructTests::LibstructTests: " + LS_TestModelFileName.string());
        }
    }
};

TEST_F(LibstructTests, testLibStructTestsDataFiles) {
    string sec("LS_TESTS");
    string key("InputFile");

    ASSERT_TRUE(iniFile.Load(LS_TestDataFileName.string()));

    //clog << "Loaded test data from file: " << testDataFileName;
    if (iniFile.GetSection(sec)) {
        IniSection *sbml = iniFile.GetSection(sec);
        IniKey *fNameKey = sbml->GetKey(key);
        if (fNameKey) {
            LS_TestModelFileName = cAPICoreModelsDir / fNameKey->mValue;
            EXPECT_TRUE(std::filesystem::exists(LS_TestModelFileName));
        }
    }
    EXPECT_TRUE(loadSBMLFromFileE(gRR, LS_TestModelFileName.string().c_str(), true));
}

TEST_F(LibstructTests, getLinkMatrix) {
    string section("getLinkMatrix");
    if (iniFile.GetSection(section)) {
        IniSection *isec = iniFile.GetSection(section);
        ls::DoubleMatrix ref = getDoubleMatrixFromString(isec->GetNonKeysAsString());

        //Testing call
        RRDoubleMatrix *actual = getLinkMatrix(gRR);

        //Check dimensions
        if (actual->RSize != ref.RSize() || actual->CSize != ref.CSize()) {
            EXPECT_TRUE(false);
            return;
        }

        for (int row = 0; row < actual->RSize; row++) {
            for (int col = 0; col < actual->CSize; col++) {
                EXPECT_NEAR(ref(row, col), actual->Data[row * actual->CSize + col], 1e-6);
            }
        }
    }
}

TEST_F(LibstructTests, getEigenValuesMatrix) {
    string section("getEigenValuesMatrix");

    if (iniFile.GetSection(section)) {
        IniSection *isec = iniFile.GetSection(section);
        ls::DoubleMatrix ref = getDoubleMatrixFromString(isec->GetNonKeysAsString());
        RRDoubleMatrix *input = getLinkMatrix(gRR);

        //Testing call
        RRDoubleMatrix *actual = getEigenvaluesMatrix(input);
        /////////////////////////////////

        //Check dimensions
        if (actual->RSize != ref.RSize() || actual->CSize != ref.CSize()) {
            EXPECT_TRUE(false);
            return;
        }

        for (int row = 0; row < actual->RSize; row++) {
            for (int col = 0; col < actual->CSize; col++) {
                EXPECT_NEAR(ref(row, col), actual->Data[row * actual->CSize + col], 1e-6);
            }
        }
    }
}

TEST_F(LibstructTests, getEigenValuesVector) {
    string section("getEigenvaluesVector");

    if (iniFile.GetSection(section)) {
        IniSection *isec = iniFile.GetSection(section);
        vector<complex<double> > ref = getComplexVectorFromString(isec->GetNonKeysAsString());
        RRDoubleMatrix *input = getLinkMatrix(gRR);
        //Testing call
        RRComplexVector *actual = getEigenvaluesVector(input);
        //////////////////////////////////////////////////////////////////

        //Check dimensions
        if (actual->Count != ref.size()) {
            EXPECT_TRUE(false);
            return;
        }

        for (int row = 0; row < actual->Count; row++) {
            EXPECT_NEAR(real(ref[row]), actual->Data[row].re, 1e-6);
            EXPECT_NEAR(imag(ref[row]), actual->Data[row].imag, 1e-6);
        }
    }
}