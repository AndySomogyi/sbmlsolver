#include <string>
#include "gtest/gtest.h"
#include "rrConfig.h"
#include "rrIniFile.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrUtils.h"
#include "wrappers/C/rrc_api.h"
#include "wrappers/C/rrc_cpp_support.h"
#include "../test_util.h"
#include <algorithm>

#include "Poco/Path.h"
#include "Poco/Glob.h"

//using..
using namespace std;
using namespace ls;
using namespace rrc;
using namespace rr;

extern string gRRTestDir;
extern string gRROutputDir;
extern RRHandle gRR;
extern IniFile iniFile;

string rrTestFileName = "";

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
  ASSERT_TRUE(jFull.RSize() == jReduced.RSize());
  ASSERT_TRUE(jFull.CSize() == jReduced.CSize());

  for(int row = 0; row < jFull.RSize(); row++)
  {
    for(int col = 0; col < jFull.CSize(); col++)
    {
      EXPECT_NEAR(jFull(row, col), jReduced(row, col), 1e-6);
    }
  }
}

void compareMatrices(const ls::DoubleMatrix& ref, const ls::DoubleMatrix& calc)
{
    clog << "Reference Matrix:" << endl;
    clog << ref << endl;

    clog << "Calculated Matrix:" << endl;
    clog << calc << endl;

    ASSERT_TRUE(calc.RSize() == ref.RSize());
    ASSERT_TRUE(calc.CSize() == ref.CSize());

    for (int i = 0; i < ref.RSize(); i++)
    {
        for (int j = 0; j < ref.CSize(); j++)
        {
            if (abs(ref(i, j) - calc(i, j)) > abs((ref(i,j)+1e-7)*1e-4))
              clog <<  "check close failed zzxx\n";
            EXPECT_NEAR(ref(i, j), calc(i, j), abs((ref(i,j)+1e-7)*1e-4));
        }
    }
}


void compareMatrices(const ls::DoubleMatrix& ref, const std::vector<ls::Complex> calc)
{
    clog<<"Reference Matrix:" << endl;
    clog<<ref<<endl;

    clog<<"Calculated Matrix:" << endl;
    for(int i = 0; i < calc.size(); ++i)
    {
        clog << calc[i] << endl;
    }

    ASSERT_TRUE(ref.CSize() == 2);
    ASSERT_TRUE(calc.size() == ref.RSize());

    for(int i = 0 ; i < ref.RSize(); i++)
    {
        EXPECT_NEAR(ref(i,0), std::real(calc[i]), abs((ref(i,0)+1e-7)*1e-4));
        EXPECT_NEAR(ref(i,1), std::imag(calc[i]), abs((ref(i,1)+1e-7)*1e-4));
    }
}

void compareMatrices(const ls::DoubleMatrix& ref, RRDoubleMatrixPtr calc)
{
    ASSERT_TRUE(calc);

    clog<<"Reference Matrix:\n";
    clog<<ref<<endl;

    clog<<"Calculated Matrix:";
    clog<<matrixToString(calc);

    ASSERT_TRUE(calc->RSize == ref.RSize());
    ASSERT_TRUE(calc->CSize == ref.CSize());

    for(int i = 0 ; i < ref.RSize(); i++)
    {
      for(int j=0; j < ref.CSize(); j++)
      {
        double val;
        if(!getMatrixElement(calc, i , j, &val))
        {
          EXPECT_TRUE(false);
        }
        EXPECT_NEAR(ref(i,j), val, abs((val+1e-7)*1e-4));
      }
    }
}

void trySteadyState(RRHandle& gRR)
{
    double val;
    for (int i=0; i<10; i++)
    {
        EXPECT_TRUE( steadyState(gRR, &val));
        if (val<1e-5) break;
    }
    EXPECT_NEAR(0, val, 1e-5);
}

int findCloseTimepoint(const DoubleMatrix* m, double t, double tol)
{
    for(int i=0; i<m->RSize(); ++i) {
        if (abs((*m)(i,0) - t) <= tol)
            return i;
    }
    return -1;
}

std::set<std::string> getTestFiles(const std::string& dir)
{
    Poco::Path path;

    if (dir.length() == 0)
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

    Log(Logger::LOG_WARNING) << "Looking in " << path.toString() << " for test files";

    //         path.setFileName("*.rrtest");

    //         std::set<std::string> files;

    //         Poco::Glob::glob(path, files);

    const char* rrtest_files[] = {
        //"Add_Species1.rrtest"
        "Bimolecular_end.rrtest",
        "Comp.rrtest",
        "Conserved_Cycle_and_Branch.rrtest",
        "Conserved_Cycle.rrtest",
        "Cycle_across_branches.rrtest",
        "Cycle.rrtest",
        "Cycle_to_Input_and_Branch.rrtest",
        "Four_Steps.rrtest",
        "Futile_Cycle.rrtest",
        "jacobian_1.rrtest",
        "Multibranch1.rrtest",
        "MultiBranch2.rrtest",
        "oscli.rrtest",
        "reversible_Jacobian.rrtest",
        "Simple_Branch.rrtest",
        "Test_1.rrtest",
        "Test_2.rrtest",
        "Test_3.rrtest",
        "Three_Steps.rrtest",
        "Two_Cycles.rrtest",
        "Two_Steps.rrtest"
        //           "variable_time_step.rrtest"
    };

    std::set<std::string> result;

    for (int n = 0; n < sizeof(rrtest_files) / sizeof(rrtest_files[0]); ++n) {
        Poco::Path testfile(path);
        testfile.append(rrtest_files[n]);
        result.insert(testfile.toString());
    }

    return result;
}

//These tests are intended to duplicate the Python tests
void check_DATA_FILES()
{
    Log(Logger::LOG_NOTICE) << "Running Test Suite TEST_MODEL on " << rrTestFileName;

    gRR = createRRInstance();

    // need to re-assign it, Load does not clear old data.;
    iniFile.Clear();

    EXPECT_TRUE(fileExists(rrTestFileName));
    EXPECT_TRUE(iniFile.Load(rrTestFileName));

    IniSection* sbmlsec = iniFile.GetSection("SBML");
    if (!sbmlsec)
    {
        EXPECT_TRUE(false);
        Log(Logger::LOG_FATAL) << "No 'SBML' section found in " << rrTestFileName;
        return;
    }
    sbmlsec->mIsUsed = true;
    string sbml = sbmlsec->GetNonKeysAsString();
    if (sbml.find('<') == string::npos)
    {
        sbml = joinPath(gRRTestDir + "rrtest_files/", sbml);
        EXPECT_TRUE(fileExists(sbml));
    }
    if (!loadSBMLEx(gRR, sbml.c_str(), true))
    {
        EXPECT_TRUE(false);
        Log(Logger::LOG_FATAL) << "Unable to load SBML:" << endl << sbml;
    }
}

void check_SET_COMPUTE_AND_ASSIGN_CONSERVATION_LAWS()
{
    EXPECT_TRUE(gRR != NULL);
    bool res = setComputeAndAssignConservationLaws(gRR, true);
    EXPECT_TRUE(res);
    clog << "\nConservation laws: " << res << endl;
}

void check_SET_STEADY_STATE_SELECTION_LIST()
{
    EXPECT_TRUE(gRR != NULL);

    //Load reference data
    IniSection* aSection = iniFile.GetSection("Set Steady State Selection List");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SET_STEADY_STATE_SELECTION_LIST ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    bool res = setSteadyStateSelectionList(gRR, keys.c_str());
    EXPECT_TRUE(res);

    //Actually calculate the steady state:
    trySteadyState(gRR);
}

void check_GET_STEADY_STATE_SELECTION_LIST()
{
    IniSection* aSection = iniFile.GetSection("Get Steady State Selection List");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_STEADY_STATE_SELECTION_LIST ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getSteadyStateSelectionList(gRR);

    if (!list)
    {
        EXPECT_TRUE(false);
        return;
    }
    vector<string> selList = splitString(keys, " ,");
    if (selList.size() != list->Count)
    {
        EXPECT_TRUE(false);
        freeStringArray(list);
        return;
    }
    for (int i = 0; i < selList.size(); i++)
    {
        EXPECT_TRUE(selList[i] == list->String[i]);
    }
    freeStringArray(list);
}

void check_SPECIES_CONCENTRATIONS()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Species Concentrations");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SPECIES_CONCENTRATIONS ====" << endl << endl;
    aSection->mIsUsed = true;

    string valstr = Trim(aSection->GetNonKeysAsString());
    vector<string> vals = splitString(valstr, " ");
    for (int i = 0; i < vals.size(); i++)
    {
        double val;

        //API Function getValue
        if (!getValue(gRR, vals[i].c_str(), &val))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(toDouble(vals[i]), val, 1e-6);
        clog << "\n";
        clog << "Ref:\t" << vals[i] << "\tActual:\t " << val << endl;
    }
}

void check_GET_SPECIES_INITIAL_CONCENTRATIONS()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Species Initial Concentrations");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_SPECIES_INITIAL_CONCENTRATIONS ====" << endl << endl;
    aSection->mIsUsed = true;

    for (int i = 0; i < aSection->KeyCount(); i++)
    {
        IniKey* aKey = aSection->GetKey(i);
        double val;
        if (!getValue(gRR, aKey->mKey.c_str(), &val))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(aKey->AsFloat(), val, 1e-6);
        clog << "\n";
        clog << "Ref:\t" << aKey->AsFloat() << "\tActual:\t " << val << endl;
    }
}

void check_GET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Species Initial Concentrations By Index");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX ====" << endl << endl;
    aSection->mIsUsed = true;

    string valstr = Trim(aSection->GetNonKeysAsString());
    vector<string> vals = splitString(valstr, " ");
    for (int i = 0; i < static_cast<int>(vals.size()); i++)
    {
        IniKey* aKey = aSection->GetKey(i);
        double val;
        if (!getFloatingSpeciesInitialConcentrationByIndex(gRR, i, &val))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(toDouble(vals[i]), val, 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(vals[i]) << "\tActual:\t " << val << endl;
    }
}

void check_GET_INITIAL_FLOATING_SPECIES_CONCENTRATIONS()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Initial Floating Species Concs");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_INITIAL_FLOATING_SPECIES_CONCENTRATIONS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRVector* values = getFloatingSpeciesInitialConcentrations(gRR);

    vector<string> refList = splitString(keys, " ,");

    if (!values || values->Count != refList.size())
    {
        EXPECT_TRUE(false);
        freeVector(values);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {

        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
}

void check_SET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Set Species Initial Concentrations By Index");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX ====" << endl << endl;
    aSection->mIsUsed = true;

    string valstr = Trim(aSection->GetNonKeysAsString());
    vector<string> vals = splitString(valstr, " ");
    for (int i = 0; i < static_cast<int>(vals.size()); i++)
    {
        //Set the value..
        double newval = toDouble(vals[i]);
        setFloatingSpeciesInitialConcentrationByIndex(gRR, i, newval);

        double val;
        //Read it back
        if (!getFloatingSpeciesInitialConcentrationByIndex(gRR, i, &val))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(newval, val, 1e-6);
        clog << "\n";
        clog << "Ref:\t" << newval << "\tActual:\t " << val << endl;
    }
}

void check_SET_SPECIES_INITIAL_CONCENTRATIONS()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Set Species Initial Concentrations");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SET_SPECIES_INITIAL_CONCENTRATIONS ====" << endl << endl;
    aSection->mIsUsed = true;

    for (int i = 0; i < aSection->KeyCount(); i++)
    {
        IniKey* aKey = aSection->GetKey(i);

        //Set the value..
        setValue(gRR, aKey->mKey.c_str(), aKey->AsFloat());

        double val;
        //Read it back
        if (!getValue(gRR, aKey->mKey.c_str(), &val))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(aKey->AsFloat(), val, 1e-6);
        clog << "\n";
        clog << "Ref:\t" << aKey->AsFloat() << "\tActual:\t " << val << endl;
    }

    //We need to set back the values to concentrations, after running steady state..
    reset(gRR);
    trySteadyState(gRR);
}

void check_STEADY_STATE_FLUXES()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Steady State Fluxes");
    if (!aSection || !gRR)
    {
        return;
    }

    clog << endl << "==== STEADY STATE FLUXES ====" << endl << endl;
    aSection->mIsUsed = true;

    reset(gRR);
    trySteadyState(gRR);

    for (int i = 0; i < aSection->KeyCount(); i++)
    {
        IniKey* aKey = aSection->GetKey(i);
        double val;

        //API Function getValue
        if (!getValue(gRR, aKey->mKey.c_str(), &val))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(aKey->AsFloat(), val, max(1e-9, abs(val * 1e-5)));
        clog << "\n";
        clog << "Ref:\t" << aKey->AsFloat() << "\tActual:\t " << val << endl;
    }
}

void check_FULL_JACOBIAN()
{
    IniSection* aSection = iniFile.GetSection("Full Jacobian");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== FULL JACOBIAN ====" << endl << endl;
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    RoadRunner* rri = castToRoadRunner(gRR);


    ls::DoubleMatrix   jActual = rri->getFullJacobian();
    ls::DoubleMatrix   jRef = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    compareMatrices(jRef, jActual);
}

void check_REDUCED_JACOBIAN()
{
    IniSection* aSection = iniFile.GetSection("Reduced Jacobian");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== REDUCED JACOBIAN ====" << endl << endl;
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    RoadRunner* rri = castToRoadRunner(gRR);

    ls::DoubleMatrix   jActual = rri->getReducedJacobian();
    ls::DoubleMatrix   jRef = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    compareMatrices(jRef, jActual);
}

void check_AMOUNT_JACOBIAN()
{
    IniSection* aSection = iniFile.GetSection("Amount Jacobian");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== AMOUNT JACOBIAN ====" << endl << endl;
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    RoadRunner* rri = castToRoadRunner(gRR);

    ls::DoubleMatrix   jActual = rri->getFullJacobian();
    ls::DoubleMatrix   jRef = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    compareMatrices(jRef, jActual);
}

void check_INDIVIDUAL_EIGENVALUES()
{
    IniSection* aSection = iniFile.GetSection("Individual Eigenvalues");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== INDIVIDUAL_EIGENVALUES ====" << endl << endl;
    aSection->mIsUsed = true;

    RoadRunner* rri = castToRoadRunner(gRR);

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    for (int i = 0; i < aSection->KeyCount(); i++)
    {
        IniKey* aKey = aSection->GetKey(i);
        clog << "\n";
        clog << "Ref_EigenValue: " << aKey->mKey << ": " << aKey->mValue << endl;

        string eigenValueLabel = "eigenReal(" + aKey->mKey + ")";
        double val = rri->getValue(eigenValueLabel.c_str());

        clog << "EigenValue " << i << ": " << val << endl;
        EXPECT_NEAR(aKey->AsFloat(), val, abs(val * 1e-4));
    }
}

void check_INDIVIDUAL_AMOUNT_EIGENVALUES()
{
    IniSection* aSection = iniFile.GetSection("Individual Amount Eigenvalues");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== INDIVIDUAL AMOUNT EIGENVALUES ====" << endl << endl;
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    RoadRunner* rri = castToRoadRunner(gRR);
    for (int i = 0; i < aSection->KeyCount(); i++)
    {
        IniKey* aKey = aSection->GetKey(i);
        clog << "\n";
        clog << "Ref_EigenValue: " << aKey->mKey << ": " << aKey->mValue << endl;

        string eigenValueLabel = "eigenReal(" + aKey->mKey + ")";
        double val = rri->getValue(eigenValueLabel.c_str());

        clog << "EigenValue " << i << ": " << val << endl;
        EXPECT_NEAR(aKey->AsFloat(), val, 1e-5);
    }
}

void check_GET_EIGENVALUE_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Eigenvalue Matrix");

    //Read in the reference data, from the ini file
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_EIGENVALUE_MATRIX ====" << endl << endl;
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    RoadRunner* rri = castToRoadRunner(gRR);
    ls::DoubleMatrix ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    std::vector<ls::Complex> eigen = rri->getFullEigenValues();
    compareMatrices(ref, eigen);
}


void check_GET_EIGENVALUE_AMOUNT_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Eigenvalue Amount Matrix");

    //Read in the reference data, from the ini file
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_EIGENVALUE AMOUNT MATRIX ====" << endl << endl;
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    RoadRunner* rri = castToRoadRunner(gRR);
    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    std::vector<ls::Complex> eigen = rri->getFullEigenValues();

    compareMatrices(ref, eigen);
}


void check_GET_REDUCED_EIGENVALUE_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Reduced Eigenvalue Matrix");

    //Read in the reference data, from the ini file
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_REDUCED_EIGENVALUE_MATRIX ====" << endl << endl;
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    RoadRunner* rri = castToRoadRunner(gRR);
    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    std::vector<ls::Complex>  eigen = rri->getReducedEigenValues();

    compareMatrices(ref, eigen);
}


void check_STOICHIOMETRY_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Stoichiometry Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== STOICHIOMETRY_MATRIX ====" << endl << endl;
    aSection->mIsUsed = true;

    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    RoadRunner* rri = castToRoadRunner(gRR);
    ls::DoubleMatrix matrix = rri->getFullStoichiometryMatrix();
    compareMatrices(ref, matrix);
}

void check_REDUCED_STOICHIOMETRY_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Reduced Stoichiometry Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== REDUCED STOICHIOMETRY_MATRIX ====" << endl << endl;
    aSection->mIsUsed = true;

    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    RoadRunner* rri = castToRoadRunner(gRR);
    ls::DoubleMatrix matrix = rri->getReducedStoichiometryMatrix();
    compareMatrices(ref, matrix);
}

void check_LINK_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Link Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== LINK_MATRIX ====" << endl << endl;
    aSection->mIsUsed = true;

    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    RRDoubleMatrixPtr matrix = getLinkMatrix(gRR);
    compareMatrices(ref, matrix);
    freeMatrix(matrix);
}

void check_UNSCALED_ELASTICITY_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Unscaled Elasticity Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << "\n==== UNSCALED_ELASTICITY_MATRIX ====\n\n";
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    RoadRunner* rri = castToRoadRunner(gRR);
    //trySteadyState(gRR);
    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    ls::DoubleMatrix  matrix = rri->getUnscaledElasticityMatrix();

    compareMatrices(ref, matrix);
}

void check_UNSCALED_ELASTICITY_AMOUNT_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Unscaled Elasticity Amount Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << "\n==== UNSCALED_ELASTICITY_AMOUNT_MATRIX ====\n\n";
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    RoadRunner* rri = castToRoadRunner(gRR);
    //trySteadyState(gRR);
    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    ls::DoubleMatrix  matrix = rri->getUnscaledElasticityMatrix();

    compareMatrices(ref, matrix);
}

void check_SCALED_ELASTICITY_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Scaled Elasticity Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << "\n==== SCALED_ELASTICITY_MATRIX ====\n\n";
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    RoadRunner* rri = castToRoadRunner(gRR);
    //trySteadyState(gRR);
    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    ls::DoubleMatrix  matrix = rri->getScaledElasticityMatrix();

    compareMatrices(ref, matrix);
}

void check_SCALED_ELASTICITY_AMOUNT_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Scaled Elasticity Amount Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << "\n==== SCALED_ELASTICITY_AMOUNT_MATRIX ====\n\n";
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    RoadRunner* rri = castToRoadRunner(gRR);
    //trySteadyState(gRR);
    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());
    ls::DoubleMatrix  matrix = rri->getScaledElasticityMatrix();

    compareMatrices(ref, matrix);
}

void check_UNSCALED_CONCENTRATION_CONTROL_MATRIX()
{

    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("UnScaled Concentration Control Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << "\n==== UNSCALED_CONCENTRATION_CONTROL_MATRIX ====\n\n";
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);

    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    reset(gRR);
    RRDoubleMatrixPtr matrix = getUnscaledConcentrationControlCoefficientMatrix(gRR);
    compareMatrices(ref, matrix);
    freeMatrix(matrix);
}

void check_SCALED_CONCENTRATION_CONTROL_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Scaled Concentration Control Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << "\n==== SCALED_CONCENTRATION_CONTROL_MATRIX ====\n\n";
    aSection->mIsUsed = true;

    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);

    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    reset(gRR);
    RRDoubleMatrixPtr matrix = getScaledConcentrationControlCoefficientMatrix(gRR);
    compareMatrices(ref, matrix);
    freeMatrix(matrix);
}

void check_UNSCALED_FLUX_CONTROL_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Unscaled Flux Control Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << "\n==== UNSCALED_FLUX_CONTROL_MATRIX ====\n\n";
    aSection->mIsUsed = true;

    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    reset(gRR);
    RRDoubleMatrixPtr matrix = getUnscaledFluxControlCoefficientMatrix(gRR);
    compareMatrices(ref, matrix);
    freeMatrix(matrix);
}

void check_SCALED_FLUX_CONTROL_MATRIX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Scaled Flux Control Matrix");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SCALED_FLUX_CONTROL_MATRIX ====" << endl << endl;
    aSection->mIsUsed = true;

    ls::DoubleMatrix     ref = getDoubleMatrixFromString(aSection->GetNonKeysAsString());

    reset(gRR);

    RRDoubleMatrixPtr matrix = getScaledFluxControlCoefficientMatrix(gRR);
    compareMatrices(ref, matrix);
    freeMatrix(matrix);
}

void check_GET_CONTROL_COEFFICIENT()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Get Control Coefficient");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_CONTROL_COEFFICIENT ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    const char* variable = refList[0].c_str();
    const char* parameter = refList[1].c_str();
    double value = toDouble(refList[2]);

    getCC(gRR, variable, parameter, &value);

    EXPECT_NEAR(toDouble(refList[2]), value, 1e-6);
    clog << "\n";
    clog << "Ref:\t" << toDouble(refList[2]) << "\tActual:\t " << value << endl;
}

void check_CHECK_RESET()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Test Reset");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_RESET ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    const char* f = refList[0].c_str();
    double f_value = toDouble(refList[1]);
    const char* k = refList[2].c_str();
    double k_value = toDouble(refList[3]);
    const char* d = refList[4].c_str();
    double d_value = toDouble(refList[5]);

    double f_value_r;
    double k_value_r;
    double d_value_r;

    setValue(gRR, f, f_value);
    setValue(gRR, k, k_value);
    setValue(gRR, d, d_value);

    reset(gRR);

    getValue(gRR, f, &f_value_r);
    getValue(gRR, k, &k_value_r);
    getValue(gRR, d, &d_value_r);

    EXPECT_TRUE(f_value != f_value_r);
    EXPECT_EQ(k_value, k_value_r);
    EXPECT_EQ(d_value, d_value_r);
}

void check_CHECK_RESETALL()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Test ResetAll");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_RESET_ALL ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    const char* k = refList[0].c_str();
    double k_value = toDouble(refList[1]);
    const char* d = refList[2].c_str();
    double d_value = toDouble(refList[3]);

    double k_value_r;
    double d_value_r;

    setValue(gRR, k, k_value);
    setValue(gRR, d, d_value);

    resetAll(gRR);

    getValue(gRR, k, &k_value_r);
    getValue(gRR, d, &d_value_r);

    EXPECT_TRUE(k_value != k_value_r);
    EXPECT_EQ(d_value, d_value_r);
}

void check_CHECK_RESETTOORIGIN()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Test ResetToOrigin");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_RESET_TO_ORIGIN ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    const char* d = refList[0].c_str();
    double d_value = toDouble(refList[1]);

    double d_value_r;

    setValue(gRR, d, d_value);

    resetToOrigin(gRR);

    getValue(gRR, d, &d_value_r);

    EXPECT_TRUE(d_value != d_value_r);
}

void check_CHECK_RK4_OUTPUT()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Check RK4 Output");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_RK4_OUTPUT ====" << endl << endl;
    aSection->mIsUsed = true;

    RoadRunner* rri = castToRoadRunner(gRR);
    SimulateOptions opt;
    opt.start = 0;
    opt.duration = 10;

    // cvode
    clog << endl << "  simulate with " << opt.start << ", " << opt.duration << ", " << opt.steps << "\n";
    const DoubleMatrix* cvode = rri->simulate(&opt);

    // rk4
    opt.setItem("integrator", "rk4");
    clog << endl << "  simulate with " << opt.start << ", " << opt.duration << ", " << opt.steps << "\n";
    const DoubleMatrix* rk4 = rri->simulate(&opt);

    for (int k = 0; k < cvode->CSize(); k++)
    {
        EXPECT_NEAR((*cvode)(cvode->RSize() - 1, k), (*rk4)(rk4->RSize() - 1, k), 1e-6);
    }
}

void check_CHECK_RK45_OUTPUT()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Check RK45 Output");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_RK45_OUTPUT ====" << endl << endl;
    aSection->mIsUsed = true;

    RoadRunner* rri = castToRoadRunner(gRR);
    SimulateOptions opt;
    opt.start = 0;
    opt.duration = 10;

    // cvode
    clog << endl << "  simulate with " << opt.start << ", " << opt.duration << ", " << opt.steps << "\n";
    const DoubleMatrix* cvode = rri->simulate(&opt);

    // rk4
    opt.setItem("integrator", "rk45");
    clog << endl << "  simulate with " << opt.start << ", " << opt.duration << ", " << opt.steps << "\n";
    const DoubleMatrix* rk45 = rri->simulate(&opt);

    for (int k = 0; k < cvode->CSize(); k++) {
        EXPECT_NEAR((*cvode)(cvode->RSize() - 1, k), (*rk45)(rk45->RSize() - 1, k), 1e-6);
    }
}

// Placeholder for testing setValues which are not implemented in C++ yet
void check_CHECK_SETVALUES()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Test SetValues");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_SETVALUES ====" << endl << endl;
    aSection->mIsUsed = true;

    EXPECT_TRUE(true);
}

void check_FLOATING_SPECIES_IDS()
{
    IniSection* aSection = iniFile.GetSection("Floating Species Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== FLOATING_SPECIES_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getFloatingSpeciesIds(gRR);

    vector<string> selList = splitString(keys, " ,");

    if (!list && selList.size())
    {
        EXPECT_TRUE(false);
        return;
    }
    EXPECT_TRUE((selList.size() == 0 && !list) || (selList.size() == list->Count));
    if (list) {
        for (int i = 0; i < selList.size(); i++)
        {
            EXPECT_TRUE(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }
}

void check_BOUNDARY_SPECIES_IDS()
{
    IniSection* aSection = iniFile.GetSection("Boundary Species Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== BOUNDARY_SPECIES_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getBoundarySpeciesIds(gRR);

    vector<string> selList = splitString(keys, " ,");

    if (!list && selList.size())
    {
        EXPECT_TRUE(false);
        return;
    }
    EXPECT_TRUE((selList.size() == 0 && !list) || (selList.size() == list->Count));
    if (list) {
        for (int i = 0; i < selList.size(); i++)
        {
            EXPECT_TRUE(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }
}

void check_GLOBAL_PARAMETER_IDS()
{
    IniSection* aSection = iniFile.GetSection("Global Parameter Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GLOBAL_PARAMETER_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getGlobalParameterIds(gRR);

    vector<string> selList = splitString(keys, " ,");

    if (!list && selList.size())
    {
        EXPECT_TRUE(false);
        return;
    }
    EXPECT_TRUE((selList.size() == 0 && !list) || (selList.size() == list->Count));
    if (list) {
        for (int i = 0; i < selList.size(); i++)
        {
            EXPECT_TRUE(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }
}

void check_COMPARTMENT_IDS()
{
    IniSection* aSection = iniFile.GetSection("Compartment Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== COMPARTMENT_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getCompartmentIds(gRR);

    if (!list)
    {
        EXPECT_TRUE(false);
        return;
    }
    vector<string> selList = splitString(keys, " ,");
    EXPECT_TRUE(selList.size() == list->Count);
    for (int i = 0; i < selList.size(); i++)
    {
        EXPECT_TRUE(selList[i] == list->String[i]);
    }
    freeStringArray(list);
}

void check_REACTION_IDS()
{
    IniSection* aSection = iniFile.GetSection("Reaction Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== REACTION_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getReactionIds(gRR);

    vector<string> selList = splitString(keys, " ,");

    if (!list && selList.size())
    {
        EXPECT_TRUE(false);
        return;
    }
    EXPECT_TRUE((selList.size() == 0 && !list) || (selList.size() == list->Count));
    if (list) {
        for (int i = 0; i < selList.size(); i++)
        {
            EXPECT_TRUE(selList[i] == list->String[i]);
        }
        freeStringArray(list);
    }
}

void check_SPECIES_INITIAL_CONDITION_IDS()
{
    IniSection* aSection = iniFile.GetSection("Species Initial Concentration Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SPECIES_INITIAL_CONDITION_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getFloatingSpeciesInitialConditionIds(gRR);

    if (!list)
    {
        EXPECT_TRUE(false);
        return;
    }
    vector<string> selList = splitString(keys, " ,");
    EXPECT_TRUE(selList.size() == list->Count);
    for (int i = 0; i < selList.size(); i++)
    {
        EXPECT_TRUE(selList[i] == list->String[i]);
    }
    freeStringArray(list);
}

void check_GET_EIGENVALUE_IDS()
{
    IniSection* aSection = iniFile.GetSection("Get Eigenvalue Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_EIGENVALUE_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getEigenvalueIds(gRR);

    if (!list)
    {
        EXPECT_TRUE(false);
        return;
    }
    vector<string> selList = splitString(keys, " ,");
    EXPECT_TRUE(selList.size() == list->Count);
    for (int i = 0; i < selList.size(); i++)
    {
        EXPECT_TRUE(selList[i] == list->String[i]);
    }
    freeStringArray(list);
}

void check_GET_RATES_OF_CHANGE_IDS()
{
    IniSection* aSection = iniFile.GetSection("Get Rates Of Change Ids");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_RATES_OF_CHANGE_IDS ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getRatesOfChangeIds(gRR);

    if (!list)
    {
        EXPECT_TRUE(false);
        return;
    }
    vector<string> selList = splitString(keys, " ,");
    EXPECT_TRUE(selList.size() == list->Count);
    for (int i = 0; i < selList.size(); i++)
    {
        EXPECT_TRUE(selList[i] == list->String[i]);
    }
    freeStringArray(list);
}

void check_SET_STEADY_STATE_SELECTION_LIST_2()
{
    EXPECT_TRUE(gRR != NULL);

    //Load reference data
    IniSection* aSection = iniFile.GetSection("Set Steady State Selection List 2");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SET_STEADY_STATE_SELECTION_LIST_2 ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    bool res = setSteadyStateSelectionList(gRR, keys.c_str());
    EXPECT_TRUE(res);
}

void check_GET_STEADY_STATE_SELECTION_LIST_2()
{
    IniSection* aSection = iniFile.GetSection("Get Steady State Selection List 2");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_STEADY_STATE_SELECTION_LIST_2 ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getSteadyStateSelectionList(gRR);

    if (!list)
    {
        EXPECT_TRUE(false);
        return;
    }
    vector<string> selList = splitString(keys, " ,");
    EXPECT_TRUE(selList.size() == list->Count);
    for (int i = 0; i < selList.size(); i++)
    {
        EXPECT_TRUE(selList[i] == list->String[i]);
    }
    freeStringArray(list);
}

void check_SET_TIME_COURSE_SELECTION_LIST()
{
    EXPECT_TRUE(gRR != NULL);

    //Load reference data
    IniSection* aSection = iniFile.GetSection("Set Time Course Selection List");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== SET_TIME_COURSE_SELECTION_LIST ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    bool res = setTimeCourseSelectionList(gRR, keys.c_str());
    EXPECT_TRUE(res);
}

void check_GET_TIME_COURSE_SELECTION_LIST()
{
    IniSection* aSection = iniFile.GetSection("Get Time Course Selection List");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_TIME_COURSE_SELECTION_LIST ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRStringArrayPtr list = getTimeCourseSelectionList(gRR);

    if (!list)
    {
        EXPECT_TRUE(false);
        return;
    }
    vector<string> selList = splitString(keys, " ,");
    EXPECT_TRUE(selList.size() == list->Count);
    for (int i = 0; i < selList.size(); i++)
    {
        EXPECT_TRUE(selList[i] == list->String[i]);
    }
    freeStringArray(list);
}

void check_COMPUTE_STEADY_STATE_VALUES()
{
    IniSection* aSection = iniFile.GetSection("Compute Steady State Values");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== COMPUTE_STEADY_STATE_VALUES ====" << endl << endl;
    aSection->mIsUsed = true;

    RRVector* values = computeSteadyStateValues(gRR);

    string valstr = Trim(aSection->GetNonKeysAsString());
    vector<string> vals = splitString(valstr, " ");
    for (int i = 0; i < vals.size(); i++)
    {
        //Check concentrations
        EXPECT_NEAR(toDouble(vals[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << vals[i] << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
}

void check_FLOATING_SPECIES_CONCENTRATIONS()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Floating Species Concentrations");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== FLOATING_SPECIES_CONCENTRATIONS ====" << endl << endl;
    aSection->mIsUsed = true;

    RRVector* values = getFloatingSpeciesConcentrations(gRR);

    string valstr = Trim(aSection->GetNonKeysAsString());
    vector<string> vals = splitString(valstr, " ");
    for (int i = 0; i < vals.size(); i++)
    {
        //Check concentrations
        EXPECT_NEAR(toDouble(vals[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << vals[i] << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
}

void check_BOUNDARY_SPECIES_CONCENTRATIONS()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Boundary Species Concentrations");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== BOUNDARY_SPECIES_CONCENTRATIONS ====" << endl << endl;
    aSection->mIsUsed = true;

    RRVector* values = getBoundarySpeciesConcentrations(gRR);

    string valstr = Trim(aSection->GetNonKeysAsString());
    vector<string> vals = splitString(valstr, " ");
    for (int i = 0; i < vals.size(); i++)
    {
        //Check concentrations
        EXPECT_NEAR(toDouble(vals[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << vals[i] << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
}

void check_GET_GLOBAL_PARAMETER_VALUES()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Global Parameter Values");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_GLOBAL_PARAMETER_VALUES ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRVector* values = getGlobalParameterValues(gRR);

    vector<string> refList = splitString(keys, " ,");

    if (!values || values->Count != refList.size())
    {
        EXPECT_TRUE(false);
        freeVector(values);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {

        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
}

void check_GET_REACTION_RATES()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Reaction Rates");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_REACTION_RATES ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRVector* values = getReactionRates(gRR);

    vector<string> refList = splitString(keys, " ,");

    if (!values || values->Count != refList.size())
    {
        EXPECT_TRUE(false && " ");
        freeVector(values);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {

        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), values->Data[i], max(1e-9, abs(values->Data[i] * 1e-4)));
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
}

void check_GET_REACTION_RATE_BY_INDEX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Reaction Rates By Index");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_REACTION_RATE_BY_INDEX ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    vector<string> refList = splitString(keys, " ,");

    if (refList.size() != getNumberOfReactions(gRR))
    {
        EXPECT_TRUE(false);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {
        double value;
        if (!getReactionRate(gRR, i, &value))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), value, 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << value << endl;
    }
}

void check_NUMBER_OF_DEPENDENT_SPECIES()
{
    IniSection* aSection = iniFile.GetSection("Number of Dependent Species");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== NUMBER_OF_DEPENDENT_SPECIES ====" << endl << endl;
    aSection->mIsUsed = true;

    string key = Trim(aSection->GetNonKeysAsString());
    int val = getNumberOfDependentSpecies(gRR);

    EXPECT_TRUE(toInt(key) == val);
}

void check_NUMBER_OF_INDEPENDENT_SPECIES()
{
    IniSection* aSection = iniFile.GetSection("Number of Independent Species");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== NUMBER_OF_INDEPENDENT_SPECIES ====" << endl << endl;
    aSection->mIsUsed = true;

    string key = Trim(aSection->GetNonKeysAsString());
    int val = getNumberOfIndependentSpecies(gRR);

    EXPECT_TRUE(toInt(key) == val);
}

void check_NUMBER_OF_RATE_RULES()
{
    IniSection* aSection = iniFile.GetSection("Number of Rate Rules");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== NUMBER_OF_RATE_RULES ====" << endl << endl;
    aSection->mIsUsed = true;

    string key = Trim(aSection->GetNonKeysAsString());
    int val = getNumberOfRules(gRR);

    EXPECT_TRUE(toInt(key) == val);
}

void check_GET_RATES_OF_CHANGE()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Rates Of Change");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_RATES_OF_CHANGE ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    RRVector* values = getRatesOfChange(gRR);

    vector<string> refList = splitString(keys, " ,");

    if (!values || values->Count != refList.size())
    {
        EXPECT_TRUE(false);
        freeVector(values);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {
        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
}

void check_GET_REACTION_RATES_EX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Reaction Rates Ex");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_REACTION_RATES_EX ====" << endl << endl;
    aSection->mIsUsed = true;

    IniKey* conc = aSection->GetKey("concentrations");
    if (!conc)
    {
        EXPECT_TRUE(false);
        return;
    }

    IniKey* refs = aSection->GetKey("rates");
    if (!refs)
    {
        EXPECT_TRUE(false);
        return;
    }

    vector<string> refList = splitString(refs->mValue, " ");

    vector<string> concList = splitString(conc->mValue, " ");
    RRVector* aVector = createVector(static_cast<int>(concList.size()));

    for (int i = 0; i < concList.size(); i++)
    {
        aVector->Data[i] = toDouble(concList[i]);
    }

    RRVector* values = getReactionRatesEx(gRR, aVector);


    if (!values || values->Count != refList.size())
    {
        EXPECT_TRUE(false);
        freeVector(values);
        freeVector(aVector);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {
        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
    freeVector(aVector);
}

void check_GET_RATES_OF_CHANGE_EX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Rates Of Change Ex");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_RATES_OF_CHANGE_EX ====" << endl << endl;
    aSection->mIsUsed = true;

    IniKey* conc = aSection->GetKey("concentrations");
    if (!conc)
    {
        EXPECT_TRUE(false);
        return;
    }

    IniKey* refs = aSection->GetKey("rates");
    if (!refs)
    {
        EXPECT_TRUE(false);
        return;
    }

    vector<string> refList = splitString(refs->mValue, " ");

    vector<string> concList = splitString(conc->mValue, " ");
    RRVector* aVector = createVector(static_cast<int>(concList.size()));

    for (int i = 0; i < concList.size(); i++)
    {
        aVector->Data[i] = toDouble(concList[i]);
    }

    RRVector* values = getRatesOfChangeEx(gRR, aVector);


    if (!values || values->Count != refList.size())
    {
        EXPECT_TRUE(false);
        freeVector(values);
        freeVector(aVector);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {
        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), values->Data[i], 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << values->Data[i] << endl;
    }
    freeVector(values);
    freeVector(aVector);
}

void check_GET_RATES_OF_CHANGE_BY_INDEX()
{
    EXPECT_TRUE(gRR != NULL);

    //Read in the reference data, from the ini file
    IniSection* aSection = iniFile.GetSection("Get Rates Of Change By Index");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== GET_RATES_OF_CHANGE_BY_INDEX ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());

    vector<string> refList = splitString(keys, " ,");

    if (refList.size() != getNumberOfFloatingSpecies(gRR))
    {
        EXPECT_TRUE(false);
        return;
    }

    for (int i = 0; i < refList.size(); i++)
    {
        double value;
        if (!getRateOfChange(gRR, i, &value))
        {
            EXPECT_TRUE(false);
        }

        //Check concentrations
        EXPECT_NEAR(toDouble(refList[i]), value, 1e-6);
        clog << "\n";
        clog << "Ref:\t" << toDouble(refList[i]) << "\tActual:\t " << value << endl;
    }
}

void check_AMOUNT_CONCENTRATION_JACOBIANS()
{
    IniSection* aSection = iniFile.GetSection("Amount/Concentration Jacobians");
    if (!aSection)
    {
        return;
    }
    clog << endl << "==== AMOUNT_CONCENTRATION_JACOBIANS ====" << endl << endl;
    aSection->mIsUsed = true;
    rr::Variant saved = Config::getValue(Config::ROADRUNNER_JACOBIAN_MODE);
    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    compareJacobians(gRR);
    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    compareJacobians(gRR);
    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, saved);
}

void check_CHECK_DEFAULT_TIME_STEP()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Check Default Time Step");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_DEFAULT_TIME_STEP ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    int NumPts = toInt(refList[0]);

    RoadRunner* rri = castToRoadRunner(gRR);

    SimulateOptions opt;
    const DoubleMatrix* result = rri->simulate(&opt);

    if (result->RSize() != NumPts)
    {
        EXPECT_TRUE(false);
        clog << "Default time step does not match" << endl;
    }
}

void check_CHECK_SIMULATE_POINTS_VS_STEPS()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Check Simulate Points vs Steps");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK SIMULATE POINTS VS STEPS ====" << endl << endl;
    // Python-only test
    aSection->mIsUsed = true;
}

void check_CHECK_MONOTONIC_TIMEPOINTS()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Check Monotonic Timepoints");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK MONOTONIC TIMEPOINTS ====" << endl << endl;

    aSection->mIsUsed = true;
    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    RoadRunner* rri = castToRoadRunner(gRR);
    SimulateOptions opt;
    opt.start = toDouble(refList.at(0));
    opt.duration = toDouble(refList.at(1));

    // For variable step
    rri->getIntegrator()->setValue("variable_step_size", true);
    const DoubleMatrix* result = rri->simulate(&opt);

    for (int k = 1; k < result->RSize(); ++k) {
        EXPECT_TRUE(((*result)(k - 1, 0) < (*result)(k, 0)));
    }

    // For fixed step
    opt.steps = toInt(refList.at(2));
    rri->getIntegrator()->setValue("variable_step_size", false);
    clog << endl << "  simulate with " << opt.start << ", " << opt.duration << ", " << opt.steps << "\n";
    result = rri->simulate(&opt);

    for (int k = 1; k < result->RSize(); ++k) {
        EXPECT_TRUE(((*result)(k - 1, 0) < (*result)(k, 0)));
    }
}

void check_CHECK_EVENT_PRE_AND_POSTFIRE_TIMEPOINTS()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Check Event Pre and Postfire Timepoints");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK EVENT PRE AND POSTFIRE TIMEPOINTS ====" << endl << endl;

    aSection->mIsUsed = true;
    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    RoadRunner* rri = castToRoadRunner(gRR);
    SimulateOptions opt;
    opt.start = 0;
    opt.duration = 10;

    // For variable step
    rri->getIntegrator()->setValue("variable_step_size", true);
    const DoubleMatrix* result = rri->simulate(&opt);

    double max_tol = 0.0001;

    for (vector<string>::const_iterator z = refList.begin(); z != refList.end(); ++z) {
        double t = toDouble(*z);
        int i = findCloseTimepoint(result, t, max_tol);
        if (i < 0 || i + 1 >= result->RSize()) {
            EXPECT_TRUE(false);
        }
        else {
            EXPECT_TRUE(abs((*result)(i + 1, 0) - (*result)(i, 0)) < max_tol);
        }
    }
}

void check_CHECK_RESETCONSERVEDTOTAL()
{
    EXPECT_TRUE(gRR != NULL);

    IniSection* aSection = iniFile.GetSection("Test ResetConservedTotal");
    if (!aSection || !gRR)
    {
        return;
    }
    clog << endl << "==== CHECK_RESET_CONSERVED_TOTAL ====" << endl << endl;
    aSection->mIsUsed = true;

    string keys = Trim(aSection->GetNonKeysAsString());
    vector<string> refList = splitString(keys, " ,");

    const char* ct = refList[0].c_str();

    double ct_val;

    RRVector* values = computeSteadyStateValues(gRR);

    setValue(gRR, ct, 1000.);

    reset(gRR);

    getValue(gRR, ct, &ct_val);

    EXPECT_TRUE(ct_val != 1000.);

    resetAll(gRR);

    freeVector(values);
}

void check_CHECK_UNUSED_TESTS()
{
    for (int i = 0; i < iniFile.GetNumberOfSections(); i++)
    {
        if (!iniFile.GetSection(i)->mIsUsed && iniFile.GetSection(i)->mName != "Conservation Laws")
        {
            EXPECT_TRUE(false);
            clog << "Unused section:\t" << iniFile.GetSection(i)->mName << endl;
        }
    }
}

void check_FREE_RR_INSTANCE()
{
    EXPECT_TRUE(freeRRInstance(gRR));
    gRR = NULL;
}

TEST(C_API_SECONDARY, RRTESTS)
{
    set<string> files = getTestFiles(gRRTestDir + "rrtest_files/");
    for (set<string>::iterator file = files.begin(); file != files.end(); file++) {
        rrTestFileName = *file;
        check_DATA_FILES();
        check_SET_COMPUTE_AND_ASSIGN_CONSERVATION_LAWS();
        check_SET_STEADY_STATE_SELECTION_LIST();
        check_GET_STEADY_STATE_SELECTION_LIST();
        check_SPECIES_CONCENTRATIONS();
        check_GET_SPECIES_INITIAL_CONCENTRATIONS();
        check_GET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX();
        check_GET_INITIAL_FLOATING_SPECIES_CONCENTRATIONS();
        check_SET_SPECIES_INITIAL_CONCENTRATION_BY_INDEX();
        check_SET_SPECIES_INITIAL_CONCENTRATIONS();
        check_STEADY_STATE_FLUXES();
        check_FULL_JACOBIAN();
        check_REDUCED_JACOBIAN();
        check_AMOUNT_JACOBIAN();
        check_INDIVIDUAL_EIGENVALUES();
        check_INDIVIDUAL_AMOUNT_EIGENVALUES();
        check_GET_EIGENVALUE_MATRIX();
        check_GET_EIGENVALUE_AMOUNT_MATRIX();
        check_GET_REDUCED_EIGENVALUE_MATRIX();
        check_STOICHIOMETRY_MATRIX();
        check_REDUCED_STOICHIOMETRY_MATRIX();
        check_LINK_MATRIX();
        check_UNSCALED_ELASTICITY_MATRIX();
        check_UNSCALED_ELASTICITY_AMOUNT_MATRIX();
        check_SCALED_ELASTICITY_MATRIX();
        check_SCALED_ELASTICITY_AMOUNT_MATRIX();
        check_UNSCALED_CONCENTRATION_CONTROL_MATRIX();
        check_SCALED_CONCENTRATION_CONTROL_MATRIX();
        check_UNSCALED_FLUX_CONTROL_MATRIX();
        check_SCALED_FLUX_CONTROL_MATRIX();
        check_GET_CONTROL_COEFFICIENT();
        check_CHECK_RESET();
        check_CHECK_RESETALL();
        check_CHECK_RESETTOORIGIN();
        check_CHECK_RK4_OUTPUT();
        check_CHECK_RK45_OUTPUT();
        check_CHECK_SETVALUES();
        check_FLOATING_SPECIES_IDS();
        check_BOUNDARY_SPECIES_IDS();
        check_GLOBAL_PARAMETER_IDS();
        check_COMPARTMENT_IDS();
        check_REACTION_IDS();
        check_SPECIES_INITIAL_CONDITION_IDS();
        check_GET_EIGENVALUE_IDS();
        check_GET_RATES_OF_CHANGE_IDS();
        check_SET_STEADY_STATE_SELECTION_LIST_2();
        check_GET_STEADY_STATE_SELECTION_LIST_2();
        check_SET_TIME_COURSE_SELECTION_LIST();
        check_GET_TIME_COURSE_SELECTION_LIST();
        check_COMPUTE_STEADY_STATE_VALUES();
        check_FLOATING_SPECIES_CONCENTRATIONS();
        check_BOUNDARY_SPECIES_CONCENTRATIONS();
        check_GET_GLOBAL_PARAMETER_VALUES();
        check_GET_REACTION_RATES();
        check_GET_REACTION_RATE_BY_INDEX();
        check_NUMBER_OF_DEPENDENT_SPECIES();
        check_NUMBER_OF_INDEPENDENT_SPECIES();
        check_NUMBER_OF_RATE_RULES();
        check_GET_RATES_OF_CHANGE();
        check_GET_REACTION_RATES_EX();
        check_GET_RATES_OF_CHANGE_EX();
        check_GET_RATES_OF_CHANGE_BY_INDEX();
        check_AMOUNT_CONCENTRATION_JACOBIANS();
        check_CHECK_DEFAULT_TIME_STEP();
        check_CHECK_SIMULATE_POINTS_VS_STEPS();
        check_CHECK_MONOTONIC_TIMEPOINTS();
        check_CHECK_EVENT_PRE_AND_POSTFIRE_TIMEPOINTS();
        check_CHECK_RESETCONSERVEDTOTAL();
        check_CHECK_UNUSED_TESTS();
        check_FREE_RR_INSTANCE();
    }
}