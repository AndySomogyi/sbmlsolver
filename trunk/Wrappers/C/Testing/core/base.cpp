#include <string>
#include "UnitTest++.h"
#include "rr_c_api.h"
#include "rrUtils.h"
using namespace std;
using namespace UnitTest;
//using namespace rr;	//We will get some clashes if this is opened up

//Add using clauses..
using rr::JoinPath;
using rr::FileExists;

extern RRHandle gRR;	//Global roadrunner C handle
extern string gSBMLModelsPath;
extern string gCompilerPath;
extern string gSupportCodeFolder;
SUITE(Base)
{
    TEST(AllocateRR)
    {
        if(!gRR)
        {
            gRR = getRRInstance();

        }

        CHECK(gRR!=NULL);	//If gRR == NULL this is a fail
		if(gRR)
		{
			setCompilerLocation(gCompilerPath.c_str());
			setSupportCodeFolder(gSupportCodeFolder.c_str());
		}
    }

    TEST(VERSIONS)
    {
    	CHECK_EQUAL(getVersion(), 			"1.0.0");
		CHECK_EQUAL(getlibSBMLVersion(), 	"5.6.0");
    }

    TEST(MODEL_FILES)	//Test that model files for the tests are present
    {
    	CHECK(FileExists(JoinPath(gSBMLModelsPath, "feedback.xml")));
    	CHECK(FileExists(JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml")));
        CHECK(FileExists(JoinPath(gSBMLModelsPath, "ss_TurnOnConservationAnalysis.xml")));
        CHECK(FileExists(JoinPath(gSBMLModelsPath, "squareWaveModel.xml")));
    }

    TEST(LOAD_SBML)
    {
        CHECK(gRR!=NULL);
        if(!gRR)
        {
	       	return;
        }

        string model = JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml");
        CHECK(loadSBMLFromFile(model.c_str()));
    }

	TEST(LISTS)
    {
    	RRListHandle myList = createRRList();
        CHECK(myList!=NULL);

        // First construct [5, 3.1415]
        RRListItemHandle myItem = createIntegerItem (5);
        addItem (myList, &myItem);
        myItem = createDoubleItem (3.1415);
        addItem (myList, &myItem);

        // Next construct [5, 3.1415, [2.7182, "Hello"]]
        myItem = createListItem (createRRList());
        addItem (myList, &myItem);
        RRListItemHandle newItem = createDoubleItem (2.7182);
        addItem (getList (myItem), &newItem);
        newItem = createStringItem ("Hello");
        addItem (getList (myItem), &newItem);

        int length = getListLength (myList);
        myItem = getListItem (myList, 0);

       	CHECK(myItem->data.iValue == 5);

        myItem = getListItem (myList, 1);
		CHECK(myItem->data.dValue == 3.1415);
        myItem = getListItem (myList, 2);

        //Item with index 1 is a litDouble!
        CHECK(isListItem(getListItem (myList, 1), litInteger) == false);
        freeRRList (myList);

        //We could check more about lists, but it seem pretty solid at this time..?
    }

}

