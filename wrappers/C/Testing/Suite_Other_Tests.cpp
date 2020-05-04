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

using namespace UnitTest;
using namespace std;
using namespace rr;
using namespace rrc;
using namespace Poco;
using namespace Poco::XML;

//using namespace Poco::XML::NodeFilter;


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



}

