#include <string>
#include "rrLogger.h"
#include "UnitTest++.h"
#include "rr_c_api.h"
#include "rrUtils.h"
#include "rrIniFile.h"
#include "TestUtils.h"

using namespace std;
using namespace UnitTest;

//Add using clauses..
using rr::JoinPath;
using rr::FileExists;

extern string   gTempFolder;
extern string   gDataOutputFolder;
extern string 	gTestDataFolder;
extern string   gRRInstallFolder;

extern bool 	gDebug;

//This tests is mimicking the Python tests
SUITE(SUITE_1)
{
string TestDataFileName 	= "TestModel_1.dat";
IniFile iniFile;
string TestModelFileName;
RRHandle gRR;

	TEST(DATA_FILES)
	{
		gTestDataFolder 			= JoinPath(gRRInstallFolder, "tests");
		string testDataFileName 	= JoinPath(gTestDataFolder, TestDataFileName);

		CHECK(FileExists(testDataFileName));
		CHECK(iniFile.Load(testDataFileName));

		clog<<"Loaded test data from file: "<< testDataFileName;
		if(iniFile.GetSection("SBML_FILES"))
		{
			rrIniSection* sbml = iniFile.GetSection("SBML_FILES");
			rrIniKey* fNameKey = sbml->GetKey("FNAME1");
			if(fNameKey)
			{
				TestModelFileName  = JoinPath(gTestDataFolder, fNameKey->mValue);
				CHECK(FileExists(TestModelFileName));
			}
		}
		CHECK(loadSBMLFromFile(TestModelFileName.c_str()));
	}

	TEST(SET_COMPUTE_AND_ASSIGN_CONSERVATION_LAWS)
	{
		gRR = getRRInstance();
		CHECK(gRR!=NULL);
		bool res = setComputeAndAssignConservationLaws(true);
		CHECK(res);
		clog<<"\nConversation laws: "<<res<<endl;
	}

	TEST(SET_STEADY_STATE_SELECTION_LIST)
	{
		gRR = getRRInstance();
		CHECK(gRR!=NULL);

		//Load reference data
		rrIniSection* aSection = iniFile.GetSection("Set Steady State Selection List");
		if(!aSection || !gRR)
		{
			CHECK(false);
			return;
		}

		rrIniKey* aKey = aSection->GetKey("list");
		if(!aKey)
		{
			CHECK(false);
			return;
		}

		bool res = setSteadyStateSelectionList(aKey->mValue.c_str());
		CHECK(res);
	}

	TEST(GET_STEADY_STATE_SELECTION_LIST)
	{
		rrIniSection* aSection = iniFile.GetSection("Get Steady State Selection List");
		if(!aSection || !gRR)
		{
			CHECK(false);
			return;
		}

		rrIniKey* aKey = aSection->GetKey("list");
		if(!aKey)
		{
			CHECK(false);
			return;
		}

		RRStringArrayHandle list = getSteadyStateSelectionList();

		if(!list)
		{
			CHECK(false);
			return;
		}
		vector<string> selList = SplitString(aKey->mValue," ,");
		CHECK(selList.size() == list->Count);
		for(int i = 0; i < selList.size(); i++)
		{
			CHECK(selList[i] == list->String[i]);
		}
	}

	TEST(COMPUTE_STEADY_STATE)
	{
		double val;
		CHECK( steadyState(val));
		CHECK_CLOSE(0, val, 1e-6);
	}

	TEST(SPECIES_CONCENTRATIONS)
	{
		gRR = getRRInstance();
		CHECK(gRR!=NULL);

		//Read in the reference data, from the ini file
		rrIniSection* aSection = iniFile.GetSection("Species Concentrations");
		if(!aSection || !gRR)
		{
			CHECK(false);
			return;
		}

		for(int i = 0 ; i < aSection->KeyCount(); i++)
		{
			rrIniKey *aKey = aSection->GetKey(i);
			double val;

			//API Function getValue
			if(!getValue(aKey->mKey.c_str(), val))
			{
				CHECK(false);
			}

			//Check concentrations
			CHECK_CLOSE(aKey->AsFloat(), val, 1e-6);
			clog<<"\n";
			clog<<"Ref:\t"<<aKey->AsFloat()<<"\tActual:\t "<<val<<endl;
		}
	}

	TEST(FLUXES)
	{
		gRR = getRRInstance();
		CHECK(gRR!=NULL);

		//Read in the reference data, from the ini file
		rrIniSection* aSection = iniFile.GetSection("Fluxes");
		if(!aSection || !gRR)
		{
			CHECK(false);
			return;
		}

		for(int i = 0 ; i < aSection->KeyCount(); i++)
		{
			rrIniKey *aKey = aSection->GetKey(i);
			double val;

			//API Function getValue
			if(!getValue(aKey->mKey.c_str(), val))
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
		rrIniSection* aSection = iniFile.GetSection("Full Jacobian");
		if(!aSection)
		{
			CHECK(false);
			return;
		}

		RRMatrixHandle 		jActual 	= getFullJacobian();
		ls::DoubleMatrix 	jRef 		= ParseFromText(aSection->GetNonKeysAsString());

		//Check dimensions
		if(jActual->RSize != jRef.RSize() || jActual->CSize != jRef.CSize())
		{
			CHECK(false);
			return;
		}

		for(int row = 0; row < jActual->RSize; row++)
		{
			for(int col = 0; col < jActual->CSize; col++)
			{
			  CHECK_CLOSE(jRef(row,col), jActual->Data[row*jActual->CSize + col], 1e-6);
			}
		}
		//Clean up...
		freeMatrix(jActual);
	}

	TEST(INDIVIDUAL_EIGENVALUES)
	{
		rrIniSection* aSection = iniFile.GetSection("Individual EigenValues");
		if(!aSection)
		{
			CHECK(false);
			return;
		}

		for(int i = 0 ; i < aSection->KeyCount(); i++)
		{
			rrIniKey *aKey = aSection->GetKey(i);
			clog<<"\n";
			clog<<"Ref_EigenValue: "<<aKey->mKey<<": "<<aKey->mValue<<endl;

			double val;

			if(!getValue(aKey->mKey.c_str(), val))
			{
				CHECK(false);
			}

			clog<<"EigenValue "<<i<<": "<<val<<endl;
			CHECK_CLOSE(aKey->AsFloat(), val, 1e-6);
		}
	}

   TEST(GET_EIGENVALUE_MATRIX)
	{
		gRR = getRRInstance();
		CHECK(gRR!=NULL);
		setComputeAndAssignConservationLaws(true);

		rrIniSection* aSection = iniFile.GetSection("Eigenvalue Matrix");


		//Read in the reference data, from the ini file
		if(!aSection || !gRR)
		{
			CHECK(false);
			return;
		}
	   ls::DoubleMatrix 	ref 		= ParseFromText(aSection->GetNonKeysAsString());

		RRMatrixHandle eigenVals = getEigenvalues();
		if(!eigenVals)
		{
			CHECK(false);
			return;
		}
		clog<<matrixToString(eigenVals);
		if(!eigenVals || eigenVals->RSize != ref.RSize())
		{
			CHECK(false);
			return;
		}

		for(int i = 0 ; i < ref.RSize(); i++)
		{
			clog<<"\n";
			clog<<"Ref_EigenValue: "<<ref(i,0)<<endl;

			double val;
			if(!getMatrixElement(eigenVals, i , 0, val))
			{
				CHECK(false);
			}
			clog<<"EigenValue "<<i<<": "<<val<<endl;
			CHECK_CLOSE(ref(i,0), val, 1e-6);

		}
		freeMatrix(eigenVals);
	}
}

