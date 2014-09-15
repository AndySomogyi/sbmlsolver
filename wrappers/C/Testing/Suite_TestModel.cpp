#include <string>
#include "Suite_TestModel.h"
#include "UnitTest++.h"
#include "rrConfig.h"
#include "rrIniFile.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrUtils.h"
#include "rrc_api.h"
#include "rrc_cpp_support.h"
#include "src/TestUtils.h"

#include "Poco/Path.h"
#include "Poco/Glob.h"

//using..
using namespace std;
using namespace UnitTest;
using namespace ls;
using namespace rrc;
using namespace rr;

extern string gTempFolder;
extern string gTestDataFolder;
extern string gCompiler;

string Trim(const string& input)
{
  string ret = trim(input, '\n');
  if (!ret.empty() && ret[0] == '"' && ret[ret.size()-1]=='"') 
  {
    ret = ret.replace(0,1,"");
    ret = ret.replace(ret.size()-1, 1, "");
  }
  return ret;
}

void compareJacobians(RRHandle gRR)
{
  RoadRunner* rri = castToRoadRunner(gRR);
  ls::DoubleMatrix    jFull     = rri->getFullJacobian();
  ls::DoubleMatrix    jReduced  = rri->getReducedJacobian();

  //Check dimensions
  if(jFull.RSize() != jReduced.RSize() || 
    jFull.CSize() != jReduced.CSize())
  {
    CHECK(false);
    return;
  }

  for(int row = 0; row < jFull.RSize(); row++)
  {
    for(int col = 0; col < jFull.CSize(); col++)
    {
      CHECK_CLOSE(jFull(row, col), jReduced(row, col), 1e-6);
    }
  }
}

void compareMatrices(const ls::DoubleMatrix& ref, const ls::DoubleMatrix& calc)
{
        clog<<"Reference Matrix:" << endl;
        clog<<ref<<endl;

        clog<<"Calculated Matrix:" << endl;
        clog<<calc<<endl;

        if(calc.RSize() != ref.RSize())
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < ref.RSize(); i++)
        {
            CHECK_CLOSE(ref(i,0), calc(i,0), 1e-5);
        }
}

//This tests is mimicking the Python tests
SUITE(TEST_MODEL)
{
    // the current test data file that the test suite is using.
    string TestDataFileName;

    // the current ini file that the test suite is using.
    IniFile iniFile;

    // global RoadRunner ptr
    RRHandle gRR = NULL;

    std::set<std::string> getTestFiles(const std::string& dir)
    {
        Poco::Path path;

        if(dir.length() == 0)
        {
            // default testing dir
            // path of current prog
            string prog = rr::getCurrentExeFolder();

            Log(Logger::LOG_NOTICE) << "prog: " << prog;

            path = Poco::Path(prog);

            Log(Logger::LOG_NOTICE) << "path: " << path.toString();

            path.makeParent();


            Log(Logger::LOG_NOTICE) << "popdir: " << path.toString();

            path.pushDirectory("testing");
        }
        else
        {
            path = Poco::Path(dir + "\\");
        }

        path.makeAbsolute();

        Log(Logger::LOG_NOTICE) << "Looking in " << path.toString() << " for test files";

        path.setFileName("*.rrtest");

        std::set<std::string> files;

        Poco::Glob::glob(path, files);

        return files;
    }

    void setTestFile(const std::string& filePath)
    {
        TestDataFileName = filePath;
    }

    TEST(DATA_FILES)
    {
        Log(Logger::LOG_NOTICE) << "Running Test Suite TEST_MODEL on " << TestDataFileName;

        gRR = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());

        // need to re-assign it, Load does not clear old data.;
        iniFile.Clear();

        CHECK(fileExists(TestDataFileName));
        CHECK(iniFile.Load(TestDataFileName));

        IniSection* sbmlsec = iniFile.GetSection("SBML");
        if(!sbmlsec)
        {
            CHECK(false);
            Log(Logger::LOG_FATAL) << "No 'SBML' section found in " << TestDataFileName;
            return;
        }
        sbmlsec->mIsUsed = true;
        string sbml = sbmlsec->GetNonKeysAsString();
        if (sbml.find('<') == string::npos)
        {
            sbml = joinPath(gTestDataFolder, trim(sbml, '\n'));
            CHECK(fileExists(sbml));
        }
        if(!loadSBMLEx(gRR, sbml.c_str(), true))
        {
            CHECK(false);
            Log(Logger::LOG_FATAL) << "Unable to load SBML:" << endl << sbml;
        }
    }

    TEST(SET_COMPUTE_AND_ASSIGN_CONSERVATION_LAWS)
    {
        CHECK(gRR!=NULL);
        bool res = setComputeAndAssignConservationLaws(gRR, true);
        CHECK(res);
        clog<<"\nConversation laws: "<<res<<endl;
    }

    TEST(SET_STEADY_STATE_SELECTION_LIST)
    {
        CHECK(gRR!=NULL);

        //Load reference data
        IniSection* aSection = iniFile.GetSection("Set Steady State Selection List");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SET_STEADY_STATE_SELECTION_LIST ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        bool res = setSteadyStateSelectionList(gRR, keys.c_str());
        CHECK(res);

        //Actually calculate the steady state:
        double val;
        CHECK( steadyState(gRR, &val));
        CHECK_CLOSE(0, val, 1e-6);
    }

    TEST(GET_STEADY_STATE_SELECTION_LIST)
    {
        IniSection* aSection = iniFile.GetSection("Get Steady State Selection List");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_STEADY_STATE_SELECTION_LIST ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getSteadyStateSelectionList(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(SPECIES_CONCENTRATIONS)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Species Concentrations");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SPECIES_CONCENTRATIONS ====" << endl << endl;
        aSection->mIsUsed = true;

        string valstr = Trim(aSection->GetNonKeysAsString());
        vector<string> vals = splitString(valstr, " "); 
        for(int i = 0 ; i < vals.size(); i++)
        {
            double val;

            //API Function getValue
            if(!getValue(gRR, vals[i].c_str(), &val))
            {
                CHECK(false);
            }

            //Check concentrations
            CHECK_CLOSE(toDouble(vals[i]), val, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<vals[i]<<"\tActual:\t "<<val<<endl;
        }
    }

    TEST(GET_SPECIES_INITIAL_CONCENTRATIONS)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Species Initial Concentrations");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_SPECIES_INITIAL_CONCENTRATIONS ====" << endl << endl;
        aSection->mIsUsed = true;

        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);
            double val;
            if(!getValue(gRR, aKey->mKey.c_str(), &val))
            {
                CHECK(false);
            }

            //Check concentrations
            CHECK_CLOSE(aKey->AsFloat(), val, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<aKey->AsFloat()<<"\tActual:\t "<<val<<endl;
        }
    }

    TEST(GET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Species Initial Concentrations By Index");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX ====" << endl << endl;
        aSection->mIsUsed = true;

        string valstr = Trim(aSection->GetNonKeysAsString());
        vector<string> vals = splitString(valstr, " "); 
        for(size_t i = 0 ; i < vals.size(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);
            double val;
            if(!getFloatingSpeciesInitialConcentrationByIndex(gRR, i, &val))
            {
                CHECK(false);
            }

            //Check concentrations
            CHECK_CLOSE(toDouble(vals[i]), val, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(vals[i])<<"\tActual:\t "<<val<<endl;
        }
    }

    TEST(SET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Set Species Initial Concentrations By Index");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX ====" << endl << endl;
        aSection->mIsUsed = true;

        string valstr = Trim(aSection->GetNonKeysAsString());
        vector<string> vals = splitString(valstr, " "); 
        for(size_t i = 0 ; i < vals.size(); i++)
        {
            //Set the value..
            double newval = toDouble(vals[i]);
            setFloatingSpeciesInitialConcentrationByIndex(gRR, i, newval);

            double val;
            //Read it back
            if(!getFloatingSpeciesInitialConcentrationByIndex(gRR, i, &val))
            {
                CHECK(false);
            }

            //Check concentrations
            CHECK_CLOSE(newval, val, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<newval<<"\tActual:\t "<<val<<endl;
        }
    }

    TEST(SET_SPECIES_INITIAL_CONCENTRATIONS)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Set Species Initial Concentrations");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SET_SPECIES_INITIAL_CONCENTRATIONS ====" << endl << endl;
        aSection->mIsUsed = true;

        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);

            //Set the value..
            setValue(gRR, aKey->mKey.c_str(), aKey->AsFloat());

            double val;
            //Read it back
            if(!getValue(gRR, aKey->mKey.c_str(), &val))
            {
                CHECK(false);
            }

            //Check concentrations
            CHECK_CLOSE(aKey->AsFloat(), val, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<aKey->AsFloat()<<"\tActual:\t "<<val<<endl;
        }

        //We need to set back the values to concentrations, after running steady state..
        double val;
        reset(gRR);
        steadyState(gRR, &val);
    }

    TEST(STEADY_STATE_FLUXES)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Steady State Fluxes");
        if(!aSection || !gRR)
        {
            return;
        }

        clog<< endl << "==== STEADY STATE FLUXES ====" << endl << endl;
        aSection->mIsUsed = true;

        double val;
        steadyState(gRR, &val);

        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);
            double val;

            //API Function getValue
            if(!getValue(gRR, aKey->mKey.c_str(), &val))
            {
                CHECK(false);
            }

            //Check concentrations
            CHECK_CLOSE(aKey->AsFloat(), val, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<aKey->AsFloat()<<"\tActual:\t "<<val<<endl;
        }
    }

    TEST(FULL_JACOBIAN)
    {
        IniSection* aSection = iniFile.GetSection("Full Jacobian");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== FULL JACOBIAN ====" << endl << endl;
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
        RoadRunner* rri = castToRoadRunner(gRR);

        ls::DoubleMatrix   jActual = rri->getFullJacobian();
        ls::DoubleMatrix   jRef    = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        compareMatrices(jActual, jRef);
    }

    TEST(AMOUNT_JACOBIAN)
    {
        IniSection* aSection = iniFile.GetSection("Amount Jacobian");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== AMOUNT JACOBIAN ====" << endl << endl;
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
        RoadRunner* rri = castToRoadRunner(gRR);

        ls::DoubleMatrix   jActual = rri->getFullJacobian();
        ls::DoubleMatrix   jRef    = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        compareMatrices(jActual, jRef);
    }

    TEST(INDIVIDUAL_EIGENVALUES)
    {
        IniSection* aSection = iniFile.GetSection("Individual Eigenvalues");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== INDIVIDUAL_EIGENVALUES ====" << endl << endl;
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
        RoadRunner* rri = castToRoadRunner(gRR);
        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);
            clog<<"\n";
            clog<<"Ref_EigenValue: "<<aKey->mKey<<": "<<aKey->mValue<<endl;

            string eigenValueLabel ="eigen(" + aKey->mKey + ")";
            double val = rri->getValue(eigenValueLabel.c_str());

            clog<<"EigenValue "<<i<<": "<<val<<endl;
            CHECK_CLOSE(aKey->AsFloat(), val, 1e-5);
        }
    }

    TEST(INDIVIDUAL_AMOUNT_EIGENVALUES)
    {
        IniSection* aSection = iniFile.GetSection("Individual Amount Eigenvalues");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== INDIVIDUAL AMOUNT EIGENVALUES ====" << endl << endl;
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
        RoadRunner* rri = castToRoadRunner(gRR);
        for(int i = 0 ; i < aSection->KeyCount(); i++)
        {
            IniKey *aKey = aSection->GetKey(i);
            clog<<"\n";
            clog<<"Ref_EigenValue: "<<aKey->mKey<<": "<<aKey->mValue<<endl;

            string eigenValueLabel ="eigen(" + aKey->mKey + ")";
            double val = rri->getValue(eigenValueLabel.c_str());

            clog<<"EigenValue "<<i<<": "<<val<<endl;
            CHECK_CLOSE(aKey->AsFloat(), val, 1e-5);
        }
    }

    TEST(GET_EIGENVALUE_MATRIX)
    {
        CHECK(gRR!=NULL);

        setComputeAndAssignConservationLaws(gRR, true);

        IniSection* aSection = iniFile.GetSection("Eigenvalue Matrix");

        //Read in the reference data, from the ini file
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_EIGENVALUE_MATRIX ====" << endl << endl;
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
        RoadRunner* rri = castToRoadRunner(gRR);
        ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
        ls::DoubleMatrix  matrix = rri->getEigenvalues();

        compareMatrices(ref, matrix);
    }


    TEST(GET_EIGENVALUE_AMOUNT_MATRIX)
    {
        CHECK(gRR!=NULL);

        setComputeAndAssignConservationLaws(gRR, true);

        IniSection* aSection = iniFile.GetSection("Eigenvalue Amount Matrix");

        //Read in the reference data, from the ini file
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_EIGENVALUE AMOUNT MATRIX ====" << endl << endl;
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
        RoadRunner* rri = castToRoadRunner(gRR);
        ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
        ls::DoubleMatrix  matrix = rri->getEigenvalues();

        compareMatrices(ref, matrix);
    }


    TEST(STOICHIOMETRY_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Stoichiometry Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== STOICHIOMETRY_MATRIX ====" << endl << endl;
        aSection->mIsUsed = true;

        ls::DoubleMatrix     ref         = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        RRDoubleMatrixPtr matrix = getStoichiometryMatrix(gRR);
        if(!matrix)
        {
            CHECK(false);
            return;
        }

        if(!matrix || matrix->RSize != ref.RSize())
        {
            CHECK(false);
            return;
        }

        clog<<"Reference Matrix:\n";
        clog<<ref<<endl;

        clog<<"Calculated Matrix:";
        clog<<matrixToString(matrix);

        for(int i = 0 ; i < ref.RSize(); i++)
        {
            double val;
            if(!getMatrixElement(matrix, i , 0, &val))
            {
                CHECK(false);
            }
            CHECK_CLOSE(ref(i,0), val, 1e-6);
        }
        freeMatrix(matrix);
      }

    TEST(LINK_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Link Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== LINK_MATRIX ====" << endl << endl;
        aSection->mIsUsed = true;

        ls::DoubleMatrix     ref         = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        RRDoubleMatrixPtr matrix = getLinkMatrix(gRR);
        if(!matrix)
        {
            CHECK(false);
            return;
        }

        if(!matrix || matrix->RSize != ref.RSize())
        {
            CHECK(false);
            return;
        }

        clog<<"Reference Matrix:\n";
        clog<<ref<<endl;

        clog<<"Calculated Matrix:";
        clog<<matrixToString(matrix);

        for(int i = 0 ; i < ref.RSize(); i++)
        {
            double val;
            if(!getMatrixElement(matrix, i , 0, &val))
            {
                CHECK(false);
            }
            CHECK_CLOSE(ref(i,0), val, 1e-6);
        }
        freeMatrix(matrix);
      }

    TEST(UNSCALED_ELASTICITY_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Unscaled Elasticity Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<<"\n==== UNSCALED_ELASTICITY_MATRIX ====\n\n";
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
        RoadRunner* rri = castToRoadRunner(gRR);
        double test;
        steadyState(gRR, &test);
        ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
        ls::DoubleMatrix  matrix = rri->getUnscaledElasticityMatrix();

        compareMatrices(ref, matrix);
      }

    TEST(UNSCALED_ELASTICITY_AMOUNT_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Unscaled Elasticity Amount Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<<"\n==== UNSCALED_ELASTICITY_AMOUNT_MATRIX ====\n\n";
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
        RoadRunner* rri = castToRoadRunner(gRR);
        double test;
        steadyState(gRR, &test);
        ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
        ls::DoubleMatrix  matrix = rri->getUnscaledElasticityMatrix();

        compareMatrices(ref, matrix);
      }

    TEST(SCALED_ELASTICITY_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Scaled Elasticity Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<<"\n==== SCALED_ELASTICITY_MATRIX ====\n\n";
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
        RoadRunner* rri = castToRoadRunner(gRR);
        double test;
        steadyState(gRR, &test);
        ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
        ls::DoubleMatrix  matrix = rri->getScaledElasticityMatrix();

        compareMatrices(ref, matrix);
      }

    TEST(SCALED_ELASTICITY_AMOUNT_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Scaled Elasticity Amount Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<<"\n==== SCALED_ELASTICITY_AMOUNT_MATRIX ====\n\n";
        aSection->mIsUsed = true;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
        RoadRunner* rri = castToRoadRunner(gRR);
        double test;
        steadyState(gRR, &test);
        ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
        ls::DoubleMatrix  matrix = rri->getScaledElasticityMatrix();

        compareMatrices(ref, matrix);
      }

    TEST(UNSCALED_CONCENTRATION_CONTROL_MATRIX)
    {

        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("UnScaled Concentration Control Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<<"\n==== UNSCALED_CONCENTRATION_CONTROL_MATRIX ====\n\n";
        aSection->mIsUsed = true;

        ls::DoubleMatrix     ref         = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        RRDoubleMatrixPtr matrix = getUnscaledConcentrationControlCoefficientMatrix(gRR);
        if(!matrix)
        {
            CHECK(false);
            return;
        }

        clog<<"Reference Matrix:\n";
        clog<<ref<<endl;

        clog<<"Calculated Matrix:";
        clog<<matrixToString(matrix);

        if(!matrix || matrix->RSize != ref.RSize())
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < ref.RSize(); i++)
        {
            double val;
            if(!getMatrixElement(matrix, i , 0, &val))
            {
                CHECK(false);
            }
            CHECK_CLOSE(ref(i,0), val, 1e-6);

        }
        freeMatrix(matrix);
    }

    TEST(SCALED_CONCENTRATION_CONTROL_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Scaled Concentration Control Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<<"\n==== SCALED_CONCENTRATION_CONTROL_MATRIX ====\n\n";
        aSection->mIsUsed = true;

        ls::DoubleMatrix     ref         = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        RRDoubleMatrixPtr matrix = getScaledConcentrationControlCoefficientMatrix(gRR);
        if(!matrix)
        {
            CHECK(false);
            return;
        }

          clog<<"Reference Matrix:\n";
        clog<<ref<<endl;

          clog<<"Calculated Matrix:";
        clog<<matrixToString(matrix);

        if(!matrix || matrix->RSize != ref.RSize())
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < ref.RSize(); i++)
        {
            double val;
            if(!getMatrixElement(matrix, i , 0, &val))
            {
                CHECK(false);
            }
            CHECK_CLOSE(ref(i,0), val, 1e-6);

        }
        freeMatrix(matrix);
      }

    TEST(UNSCALED_FLUX_CONTROL_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Unscaled Flux Control Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<<"\n==== UNSCALED_FLUX_CONTROL_MATRIX ====\n\n";
        aSection->mIsUsed = true;

        ls::DoubleMatrix     ref         = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        RRDoubleMatrixPtr matrix = getUnscaledFluxControlCoefficientMatrix(gRR);
        if(!matrix)
        {
            CHECK(false);
            return;
        }

          clog<<"Reference Matrix:\n";
        clog<<ref<<endl;

          clog<<"Calculated Matrix:";
        clog<<matrixToString(matrix);

        if(!matrix || matrix->RSize != ref.RSize())
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < ref.RSize(); i++)
        {
            double val;
            if(!getMatrixElement(matrix, i , 0, &val))
            {
                CHECK(false);
            }
            CHECK_CLOSE(ref(i,0), val, 1e-6);

        }
        freeMatrix(matrix);
      }

    TEST(SCALED_FLUX_CONTROL_MATRIX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Scaled Flux Control Matrix");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SCALED_FLUX_CONTROL_MATRIX ====" << endl << endl;
        aSection->mIsUsed = true;

        ls::DoubleMatrix     ref         = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

        RRDoubleMatrixPtr matrix = getScaledFluxControlCoefficientMatrix(gRR);
        if(!matrix)
        {
            CHECK(false);
            return;
        }

          clog<<"Reference Matrix:\n";
        clog<<ref<<endl;

          clog<<"Calculated Matrix:";
        clog<<matrixToString(matrix);

        if(!matrix || matrix->RSize != ref.RSize())
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < ref.RSize(); i++)
        {
            double val;
            if(!getMatrixElement(matrix, i , 0, &val))
            {
                CHECK(false);
            }
            CHECK_CLOSE(ref(i,0), val, 1e-6);

        }
        freeMatrix(matrix);
      }

    TEST(FLOATING_SPECIES_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Floating Species Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== FLOATING_SPECIES_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getFloatingSpeciesIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(BOUNDARY_SPECIES_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Boundary Species Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== BOUNDARY_SPECIES_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getBoundarySpeciesIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(GLOBAL_PARAMETER_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Global Parameter Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GLOBAL_PARAMETER_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getGlobalParameterIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(COMPARTMENT_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Compartment Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== COMPARTMENT_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getCompartmentIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(REACTION_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Reaction Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== REACTION_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getReactionIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(SPECIES_INITIAL_CONDITION_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Species Initial Concentration Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SPECIES_INITIAL_CONDITION_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getFloatingSpeciesInitialConditionIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(GET_EIGENVALUE_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Get Eigenvalue Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_EIGENVALUE_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getEigenvalueIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(GET_RATES_OF_CHANGE_IDS)
    {
        IniSection* aSection = iniFile.GetSection("Get Rates Of Change Ids");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_RATES_OF_CHANGE_IDS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getRatesOfChangeIds(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(SET_STEADY_STATE_SELECTION_LIST_2)
    {
        CHECK(gRR!=NULL);

        //Load reference data
        IniSection* aSection = iniFile.GetSection("Set Steady State Selection List 2");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SET_STEADY_STATE_SELECTION_LIST_2 ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        bool res = setSteadyStateSelectionList(gRR, keys.c_str());
        CHECK(res);
    }

    TEST(GET_STEADY_STATE_SELECTION_LIST_2)
    {
        IniSection* aSection = iniFile.GetSection("Get Steady State Selection List 2");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_STEADY_STATE_SELECTION_LIST_2 ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getSteadyStateSelectionList(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(SET_TIME_COURSE_SELECTION_LIST)
    {
        CHECK(gRR!=NULL);

        //Load reference data
        IniSection* aSection = iniFile.GetSection("Set Time Course Selection List");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== SET_TIME_COURSE_SELECTION_LIST ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        bool res = setTimeCourseSelectionList(gRR, keys.c_str());
        CHECK(res);
    }

    TEST(GET_TIME_COURSE_SELECTION_LIST)
    {
        IniSection* aSection = iniFile.GetSection("Get Time Course Selection List");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_TIME_COURSE_SELECTION_LIST ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRStringArrayPtr list = getTimeCourseSelectionList(gRR);

        if(!list)
        {
            CHECK(false);
            return;
        }
        vector<string> selList = splitString(keys," ,");
        CHECK(selList.size() == list->Count);
        for(int i = 0; i < selList.size(); i++)
        {
            CHECK(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }

    TEST(COMPUTE_STEADY_STATE_VALUES)
    {
        IniSection* aSection = iniFile.GetSection("Compute Steady State Values");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== COMPUTE_STEADY_STATE_VALUES ====" << endl << endl;
        aSection->mIsUsed = true;

        RRVector* values = computeSteadyStateValues(gRR);

        string valstr = Trim(aSection->GetNonKeysAsString());
        vector<string> vals = splitString(valstr, " "); 
        for(int i = 0 ; i < vals.size(); i++)
        {
            //Check concentrations
            CHECK_CLOSE(toDouble(vals[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<vals[i]<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
    }

    TEST(FLOATING_SPECIES_CONCENTRATIONS)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Floating Species Concentrations");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== FLOATING_SPECIES_CONCENTRATIONS ====" << endl << endl;
        aSection->mIsUsed = true;

        RRVector* values = getFloatingSpeciesConcentrations(gRR);

        string valstr = Trim(aSection->GetNonKeysAsString());
        vector<string> vals = splitString(valstr, " "); 
        for(int i = 0 ; i < vals.size(); i++)
        {
            //Check concentrations
            CHECK_CLOSE(toDouble(vals[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<vals[i]<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
    }

    TEST(BOUNDARY_SPECIES_CONCENTRATIONS)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Boundary Species Concentrations");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== BOUNDARY_SPECIES_CONCENTRATIONS ====" << endl << endl;
        aSection->mIsUsed = true;

        RRVector* values = getBoundarySpeciesConcentrations(gRR);

        string valstr = Trim(aSection->GetNonKeysAsString());
        vector<string> vals = splitString(valstr, " "); 
        for(int i = 0 ; i < vals.size(); i++)
        {
            //Check concentrations
            CHECK_CLOSE(toDouble(vals[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<vals[i]<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
    }

    TEST(GET_GLOBABL_PARAMETER_VALUES)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Global Parameter Values");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_GLOBABL_PARAMETER_VALUES ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRVector* values = getGlobalParameterValues(gRR);

        vector<string> refList = splitString(keys," ,");

        if(!values || values->Count != refList.size())
        {
            CHECK(false);
            freeVector(values);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {

            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
    }

    TEST(GET_INITIAL_FLOATING_SPECIES_CONCENTRATIONS)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Initial Floating Species Concs");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_INITIAL_FLOATING_SPECIES_CONCENTRATIONS ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRVector* values = getFloatingSpeciesInitialConcentrations(gRR);

        vector<string> refList = splitString(keys," ,");

        if(!values || values->Count != refList.size())
        {
            CHECK(false);
            freeVector(values);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {

            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
    }

    TEST(GET_REACTION_RATES)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Reaction Rates");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_REACTION_RATES ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRVector* values = getReactionRates(gRR);

        vector<string> refList = splitString(keys," ,");

        if(!values || values->Count != refList.size())
        {
            CHECK(false);
            freeVector(values);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {

            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
    }

    TEST(GET_REACTION_RATE_BY_INDEX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Reaction Rates By Index");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_REACTION_RATE_BY_INDEX ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        vector<string> refList = splitString(keys," ,");

        if(refList.size() != getNumberOfReactions(gRR))
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {
            double value;
              if(!getReactionRate(gRR, i, &value))
              {
                CHECK(false);
              }

            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), value, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<value<<endl;
        }
    }

    TEST(NUMBER_OF_DEPENDENT_SPECIES)
    {
        IniSection* aSection = iniFile.GetSection("Number of Dependent Species");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== NUMBER_OF_DEPENDENT_SPECIES ====" << endl << endl;
        aSection->mIsUsed = true;

        string key = Trim(aSection->GetNonKeysAsString());
        int val = getNumberOfDependentSpecies(gRR);

        CHECK(toInt(key) ==  val);
    }

    TEST(NUMBER_OF_INDEPENDENT_SPECIES)
    {
        IniSection* aSection = iniFile.GetSection("Number of Independent Species");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== NUMBER_OF_INDEPENDENT_SPECIES ====" << endl << endl;
        aSection->mIsUsed = true;

        string key = Trim(aSection->GetNonKeysAsString());
        int val = getNumberOfIndependentSpecies(gRR);

        CHECK(toInt(key) ==  val);
    }

    TEST(NUMBER_OF_RULES)
    {
        IniSection* aSection = iniFile.GetSection("Number of Rules");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== NUMBER_OF_RULES ====" << endl << endl;
        aSection->mIsUsed = true;

        string key = Trim(aSection->GetNonKeysAsString());
        int val = getNumberOfRules(gRR);

        CHECK(toInt(key) ==  val);
    }

    TEST(GET_RATES_OF_CHANGE)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Rates Of Change");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_RATES_OF_CHANGE ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        RRVector* values = getRatesOfChange(gRR);

        vector<string> refList = splitString(keys," ,");

        if(!values || values->Count != refList.size())
        {
            CHECK(false);
            freeVector(values);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {
            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
    }

    TEST(GET_REACTION_RATES_EX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Reaction Rates Ex");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_REACTION_RATES_EX ====" << endl << endl;
        aSection->mIsUsed = true;

        IniKey* conc = aSection->GetKey("concentrations");
        if(!conc)
        {
            CHECK(false);
            return;
        }

        IniKey* refs = aSection->GetKey("rates");
        if(!refs)
        {
            CHECK(false);
            return;
        }

          vector<string> refList = splitString(refs->mValue," ");

          vector<string> concList = splitString(conc->mValue," ");
          RRVector* aVector = createVector(concList.size());

          for(int i = 0; i < concList.size(); i++)
          {
            aVector->Data[i] = toDouble(concList[i]);
          }

        RRVector* values = getReactionRatesEx(gRR, aVector);


        if(!values || values->Count != refList.size())
        {
            CHECK(false);
            freeVector(values);
            freeVector(aVector);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {
            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
        freeVector(aVector);
    }

    TEST(GET_RATES_OF_CHANGE_EX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Rates Of Change Ex");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_RATES_OF_CHANGE_EX ====" << endl << endl;
        aSection->mIsUsed = true;

        IniKey* conc = aSection->GetKey("concentrations");
        if(!conc)
        {
            CHECK(false);
            return;
        }

        IniKey* refs = aSection->GetKey("rates");
        if(!refs)
        {
            CHECK(false);
            return;
        }

          vector<string> refList = splitString(refs->mValue," ");

          vector<string> concList = splitString(conc->mValue," ");
          RRVector* aVector = createVector(concList.size());

          for(int i = 0; i < concList.size(); i++)
          {
            aVector->Data[i] = toDouble(concList[i]);
          }

          RRVector* values = getRatesOfChangeEx(gRR, aVector);


        if(!values || values->Count != refList.size())
        {
            CHECK(false);
            freeVector(values);
            freeVector(aVector);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {
            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), values->Data[i], 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<values->Data[i]<<endl;
        }
        freeVector(values);
        freeVector(aVector);
    }

    TEST(GET_RATES_OF_CHANGE_BY_INDEX)
    {
        CHECK(gRR!=NULL);

        //Read in the reference data, from the ini file
        IniSection* aSection = iniFile.GetSection("Get Rates Of Change By Index");
        if(!aSection || !gRR)
        {
            return;
        }
        clog<< endl << "==== GET_RATES_OF_CHANGE_BY_INDEX ====" << endl << endl;
        aSection->mIsUsed = true;

        string keys = Trim(aSection->GetNonKeysAsString());

        vector<string> refList = splitString(keys," ,");

        if(refList.size() != getNumberOfFloatingSpecies(gRR))
        {
            CHECK(false);
            return;
        }

        for(int i = 0 ; i < refList.size(); i++)
        {
            double value;
            if(!getRateOfChange(gRR, i, &value))
            {
                CHECK(false);
            }

            //Check concentrations
            CHECK_CLOSE(toDouble(refList[i]), value, 1e-6);
            clog<<"\n";
            clog<<"Ref:\t"<<toDouble(refList[i])<<"\tActual:\t "<<value<<endl;
        }
    }

    TEST(AMOUNT_CONCENTRATION_JACOBIANS)
    {
        IniSection* aSection = iniFile.GetSection("Amount/Concentration Jacobians");
        if(!aSection)
        {
            return;
        }
        clog<< endl << "==== AMOUNT_CONCENTRATION_JACOBIANS ====" << endl << endl;
        aSection->mIsUsed = true;
        rr::Variant saved = Config::getValue(Config::ROADRUNNER_JACOBIAN_MODE);
        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
        compareJacobians(gRR);
        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
        compareJacobians(gRR);
        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, saved);
    }

    TEST(CHECK_UNUSED_TESTS)
    {
        for(int i=0; i<iniFile.GetNumberOfSections(); i++) 
        {
            if (!iniFile.GetSection(i)->mIsUsed)
            {
                CHECK(false);
                clog << "Unused section:\t" << iniFile.GetSection(i)->mName << endl;
            }
        }
    }

    TEST(FREE_RR_INSTANCE)
    {
        CHECK(freeRRInstance(gRR));
        gRR = NULL;
    }

}
