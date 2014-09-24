#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
#include "rrIniFile.h"
#include "rrTestUtils.h"
#include "rrUtils.h"

using namespace UnitTest;
using namespace rr;
using namespace std;

extern RoadRunner*         gRR;
extern string             gTestDataFolder;
extern string             gCompiler;
extern string             gSupportCodeFolder;
extern string             gTempFolder;
extern string             gRRInstallFolder;


SUITE(SteadyState)
{
//Global to this unit
RoadRunner *aRR = NULL;
string TestDataFileName     = "TestModel_1.dat";
IniFile iniFile;
string TestModelFileName;

    //This test-suite tests various steady state functions, using the model TestModel_1.xml

    //Test that model files and reference data for the tests in this suite are present
    TEST(DATA_FILES)
    {
        gTestDataFolder     = joinPath(gRRInstallFolder, "tests");
        TestDataFileName     = joinPath(gTestDataFolder, TestDataFileName);

        CHECK(fileExists(TestDataFileName));
        CHECK(iniFile.Load(TestDataFileName));
        clog<<"Loaded test data from file: "<< TestDataFileName;
        if(iniFile.GetSection("SBML_FILES"))
        {
            IniSection* sbml = iniFile.GetSection("SBML_FILES");
            IniKey* fNameKey = sbml->GetKey("FNAME1");
            if(fNameKey)
            {
                TestModelFileName  = joinPath(gTestDataFolder, fNameKey->mValue);
                CHECK(fileExists(TestModelFileName));
            }
        }
    }

    TEST(LOAD_MODEL)
    {
        aRR = new RoadRunner(gCompiler, gTempFolder, gSupportCodeFolder);
        CHECK(aRR!=NULL);

        //Load the model
        aRR->setConservedMoietyAnalysis(true);
        aRR->load(TestModelFileName);
    }

    TEST(COMPUTE_STEADY_STATE)
    {
        //Compute Steady state
        if(aRR)
        {
            CHECK_CLOSE(0, aRR->steadyState(), 1e-6);
        }
        else
        {
            CHECK(false);
        }
    }

    TEST(STEADY_STATE_CONCENTRATIONS)
    {
        IniSection* aSection = iniFile.GetSection("STEADY_STATE_CONCENTRATIONS");
        //Read in the reference data, from the ini file
        if(!aSection || !aRR)
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);
            double val = aRR->getValue(aKey->mKey);

            //Check concentrations
            CHECK_CLOSE(aKey->AsFloat(), val, 1e-6);
        }
    }

    //This test is using the function getValue("eigen_...")
    TEST(GET_EIGENVALUES_1)
    {
        IniSection* aSection = iniFile.GetSection("EIGEN_VALUES");
        //Read in the reference data, from the ini file
        if(!aSection || !aRR)
        {
            CHECK(false);
            return;
        }

        vector<string> ids = aRR->getEigenValueIds();
        if(ids.size() != aSection->KeyCount())
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            //Find correct eigenValue
            for(int j = 0; j < ids.size(); j++)
            {
                if(aSection->mKeys[i]->mKey == ids[j])
                {
                    IniKey *aKey = aSection->GetKey(i);
                    clog<<"\n";
                    clog<<"Ref_EigenValue: "<<aKey->mKey<<": "<<aKey->mValue<<endl;

                    clog<<"ID: "<<ids[j]<<"= "<<aRR->getValue(ids[j])<<endl;

                    CHECK_CLOSE(aKey->AsFloat(), aRR->getValue(ids[j]), 1e-6);
                }
            }
        }
    }

    TEST(GET_EIGENVALUES_2)
    {
        IniSection* aSection = iniFile.GetSection("EIGEN_VALUES");
        //Read in the reference data, from the ini file
        if(!aSection || !aRR)
        {
            CHECK(false);
            return;
        }

        DoubleMatrix eigenVals = aRR->getFullEigenValues();
        if(eigenVals.RSize() != aSection->KeyCount())
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);
            clog<<"\n";
            clog<<"Ref_EigenValue: "<<aKey->mKey<<": "<<aKey->mValue<<endl;

            double eigenVal = eigenVals(i,0);

            clog<<"EigenValue "<<i<<": "<< eigenVal << endl;
            CHECK_CLOSE(aKey->AsFloat(), eigenVal, 1e-6);
        }
    }

    TEST(FULL_JACOBIAN)
    {
        IniSection* aSection = iniFile.GetSection("FULL_JACOBIAN");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        DoubleMatrix fullJacobian     = aRR->getFullJacobian();
        DoubleMatrix jRef             = ParseMatrixFromText(aSection->GetNonKeysAsString());

        //Check dimensions
        if(fullJacobian.RSize() != jRef.RSize() || fullJacobian.CSize() != jRef.CSize())
        {
            CHECK(false);
            return;
        }

        clog<<"Full Jacobian\n"<<fullJacobian;
        CHECK_ARRAY2D_CLOSE(jRef, fullJacobian, fullJacobian.RSize(),fullJacobian.CSize(), 1e-6);
    }

    TEST(REDUCED_JACOBIAN)
    {
        IniSection* aSection = iniFile.GetSection("REDUCED_REORDERED_JACOBIAN");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        DoubleMatrix fullJacobian     = aRR->getReducedJacobian();
        DoubleMatrix jRef             = ParseMatrixFromText(aSection->GetNonKeysAsString());

        //Check dimensions
        if(fullJacobian.RSize() != jRef.RSize() || fullJacobian.CSize() != jRef.CSize())
        {
            CHECK(false);
            return;
        }
        clog<<"Reduced Jacobian\n"<<fullJacobian;
        CHECK_ARRAY2D_CLOSE(jRef, fullJacobian, fullJacobian.RSize(),fullJacobian.CSize(), 1e-6);
    }

    TEST(FULL_REORDERED_JACOBIAN)
    {
        IniSection* aSection = iniFile.GetSection("FULL_REORDERED_JACOBIAN");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        //Read in the reference data, from the ini file
        DoubleMatrix matrix = aRR->getFullReorderedJacobian();
        DoubleMatrix ref = ParseMatrixFromText(aSection->GetNonKeysAsString());

        cout<<"Reference\n"<<ref;
        cout<<"matrix\n"<<matrix;

        //Check dimensions
        if(matrix.RSize() != ref.RSize() || matrix.CSize() != ref.CSize())
        {
            CHECK(false);
            return;
        }

        CHECK_ARRAY2D_CLOSE(ref, matrix, matrix.RSize(), matrix.CSize(), 1e-6);
    }

    TEST(REDUCED_REORDERED_JACOBIAN)
    {
        IniSection* aSection = iniFile.GetSection("FULL_REORDERED_JACOBIAN");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        //Read in the reference data, from the ini file
        DoubleMatrix matrix = aRR->getReducedJacobian();
        DoubleMatrix ref = ParseMatrixFromText(aSection->GetNonKeysAsString());

        cout<<"Reference\n"<<ref;
        cout<<"matrix\n"<<matrix;

        //Check dimensions
        if(matrix.RSize() != ref.RSize() || matrix.CSize() != ref.CSize())
        {
            CHECK(false);
            return;
        }

        CHECK_ARRAY2D_CLOSE(ref, matrix, matrix.RSize(), matrix.CSize(), 1e-6);
    }

    TEST(LINK_MATRIX)
    {
        IniSection* aSection = iniFile.GetSection("LINK_MATRIX");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        //Read in the reference data, from the ini file
        DoubleMatrix matrix     = aRR->getLinkMatrix();
        DoubleMatrix ref        = ParseMatrixFromText(aSection->GetNonKeysAsString());

        //Check dimensions
        if(matrix.RSize() != ref.RSize() || matrix.CSize() != ref.CSize())
        {
            CHECK(false);
            return;
        }

        CHECK_ARRAY2D_CLOSE(ref, matrix, matrix.RSize(), matrix.CSize(), 1e-6);
    }

    TEST(UNSCALED_ELASTICITY_MATRIX)
    {
        IniSection* aSection = iniFile.GetSection("UNSCALED_ELASTICITY_MATRIX");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        //Read in the reference data, from the ini file
        DoubleMatrix matrix     = aRR->getUnscaledElasticityMatrix();
        DoubleMatrix ref          = ParseMatrixFromText(aSection->GetNonKeysAsString());

        //Check dimensions
        if(matrix.RSize() != ref.RSize() || matrix.CSize() != ref.CSize())
        {
            CHECK(!"Wrong matrix dimensions" );
            return;
        }

        CHECK_ARRAY2D_CLOSE(ref, matrix, matrix.RSize(), matrix.CSize(), 1e-6);
    }

    TEST(SCALED_ELASTICITY_MATRIX)
    {
        IniSection* aSection = iniFile.GetSection("SCALED_ELASTICITY_MATRIX");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        //Read in the reference data, from the ini file
        DoubleMatrix matrix     = aRR->getScaledElasticityMatrix();
        DoubleMatrix ref          = ParseMatrixFromText(aSection->GetNonKeysAsString());

        //Check dimensions
        if(matrix.RSize() != ref.RSize() || matrix.CSize() != ref.CSize())
        {
            CHECK(!"Wrong matrix dimensions" );
            return;
        }

        CHECK_ARRAY2D_CLOSE(ref, matrix, matrix.RSize(), matrix.CSize(), 1e-6);
    }

    TEST(UNSCALED_CONCENTRATION_CONTROL_MATRIX)
    {
        IniSection* aSection = iniFile.GetSection("UNSCALED_CONCENTRATION_CONTROL_MATRIX");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        //Read in the reference data, from the ini file
        DoubleMatrix matrix     = aRR->getUnscaledConcentrationControlCoefficientMatrix();
        DoubleMatrix ref          = ParseMatrixFromText(aSection->GetNonKeysAsString());

        //Check dimensions
        if(matrix.RSize() != ref.RSize() || matrix.CSize() != ref.CSize())
        {
            CHECK(!"Wrong matrix dimensions" );
            return;
        }

        CHECK_ARRAY2D_CLOSE(ref, matrix, matrix.RSize(), matrix.CSize(), 1e-6);
    }

    TEST(UNSCALED_FLUX_CONTROL_MATRIX)
    {
        IniSection* aSection = iniFile.GetSection("UNSCALED_FLUX_CONTROL_MATRIX");
           if(!aSection)
        {
            CHECK(false);
            return;
        }

        //Read in the reference data, from the ini file
        DoubleMatrix matrix     = aRR->getUnscaledFluxControlCoefficientMatrix();
        DoubleMatrix ref          = ParseMatrixFromText(aSection->GetNonKeysAsString());

        //Check dimensions
        if(matrix.RSize() != ref.RSize() || matrix.CSize() != ref.CSize())
        {
            CHECK(!"Wrong matrix dimensions" );
            return;
        }

        CHECK_ARRAY2D_CLOSE(ref, matrix, matrix.RSize(), matrix.CSize(), 1e-6);
    }

    TEST(FREE_ROADRUNNER)
    {
        delete aRR;
    }
}

