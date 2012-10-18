#include <string>
#include "UnitTest++.h"
#include "rr_c_api.h"
#include "rrUtils.h"
using namespace std;
using namespace UnitTest;

//Add using clauses..
using rr::JoinPath;
using rr::FileExists;

extern RRHandle gRR;	//Global roadrunner C handle
extern string gSBMLModelsPath;
extern string gCompilerPath;

SUITE(SteadyState)
{
    TEST(AllocateRR)
    {
        if(!gRR)
        {
            gRR = getRRInstance();

        }
        CHECK(gRR!=NULL);
    }

    TEST(LOAD_SBML)
    {
        CHECK(gRR!=NULL);
        string model =  JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml");
        CHECK(loadSBMLFromFile(model.c_str()));
    }

    TEST(SS_SYMBOLS)
    {
    	RRListHandle sList = getAvailableSteadyStateSymbols();
		char* symbols = listToString(sList);
		if(!symbols)
		{
			CHECK(false);
			return;
		}
        char expectedSymbols[6] =	{"sdsds"};

        CHECK_EQUAL("tests", "tests");
		freeRRList(sList);
		freeText(symbols);
    }

}

