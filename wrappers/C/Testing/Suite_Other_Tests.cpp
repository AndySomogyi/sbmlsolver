#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Poco/File.h"
#include "Poco/DOM/ProcessingInstruction.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/DOM/TreeWalker.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/MD5Engine.h"
#include "unit_test/UnitTest++.h"
#include "rrc_api.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"

using namespace UnitTest;
using namespace std;
using namespace rr;
using namespace rrc;
using namespace Poco;
using namespace Poco::XML;

//using namespace Poco::XML::NodeFilter;

using namespace std;

extern string   gTempFolder;
extern string   gTestDataFolder;
extern string   gCompiler;


string getListOfReactionsText(const string& fName);

SUITE(OTHER_TESTS)
{
    TEST(EMPTY_EVENT_ASSIGNMENTS)
    {
        // Event assignments in L3v2 can be empty
        RRHandle aRR = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        string TestModelFileName = joinPath(gTestDataFolder, "eventassignment_nomath.xml");
        CHECK(fileExists(TestModelFileName));

        CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), true));
        CHECK(simulate(aRR));


    }

    TEST(SAVED_SPECIES_AMOUNT)
    {
        string TestModelFileName = joinPath(gTestDataFolder, "species_conc.xml");
        CHECK(fileExists(TestModelFileName));
        RoadRunner rr(TestModelFileName, NULL);
        CHECK(rr.getFloatingSpeciesByIndex(0) == 5.0);
        string sbml = rr.getCurrentSBML();
        RoadRunner rr2(sbml, NULL);
        CHECK(rr2.getFloatingSpeciesByIndex(0) == 5.0);
        sbml = rr2.getCurrentSBML();
        RoadRunner rr3(sbml, NULL);
        CHECK(rr3.getFloatingSpeciesByIndex(0) == 5.0);
    }

    TEST(SPECIES_UNITS)
    {
        string TestModelFileName = joinPath(gTestDataFolder, "species_hosu1.xml");
        CHECK(fileExists(TestModelFileName));
        RoadRunner rr(TestModelFileName, NULL);
        CHECK(rr.getFloatingSpeciesByIndex(0) == 3.0);
        string sbml = rr.getCurrentSBML();
        RoadRunner rr2(sbml, NULL);
        CHECK(rr2.getFloatingSpeciesByIndex(0) == 3.0);
        sbml = rr2.getCurrentSBML();
        RoadRunner rr3(sbml, NULL);
        CHECK(rr3.getFloatingSpeciesByIndex(0) == 3.0);
    }

    TEST(SPECIES_UNITS2)
    {
        string TestModelFileName = joinPath(gTestDataFolder, "species_hosu2.xml");
        CHECK(fileExists(TestModelFileName));
        RoadRunner rr(TestModelFileName, NULL);
        CHECK(rr.getFloatingSpeciesByIndex(0) == 3.0);
        string sbml = rr.getCurrentSBML();
        RoadRunner rr2(sbml, NULL);
        CHECK(rr2.getFloatingSpeciesByIndex(0) == 3.0);
        sbml = rr2.getCurrentSBML();
        RoadRunner rr3(sbml, NULL);
        CHECK(rr3.getFloatingSpeciesByIndex(0) == 3.0);
    }


}

