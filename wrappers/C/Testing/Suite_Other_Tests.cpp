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

    TEST(ADD_NEW_UNIT)
    {
        string TestModelFileName = joinPath(gTestDataFolder, "mole_unit.xml");
        CHECK(fileExists(TestModelFileName));
        RoadRunner rr(TestModelFileName, NULL);
        
        // this should pass since mole is defined by the user
        rr.addSpecies("S1", "c1", 2.0, "mole", true);

        // this should pass since substance is defined by SBML
        rr.addSpecies("S2", "c1", 2.0, "substance", true);
        
        try {
            // this should fail since concentration is not defined by the user
            rr.addSpecies("S3", "c1", 2.0, "concentration", true);
            CHECK(false);
        }
        catch (rr::Exception e) {
            Log(rr::Logger::LOG_DEBUG) << "Caught exception as expected:"  << e.what();
        }
    }
}

