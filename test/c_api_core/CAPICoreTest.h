#ifndef ROADRUNNER_CAPICORETEST
#define ROADRUNNER_CAPICORETEST

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
#include "gtest/gtest.h"
#include "C/rrc_api.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLogger.h"

#include "rrRoadRunner.h"
#include "RoadRunnerTest.h"
#include <filesystem>

//using namespace std;
using namespace rr;
using namespace rrc;
using namespace Poco;
using namespace Poco::XML;
using std::filesystem::path;


class CAPICoreTest : public RoadRunnerTest {
public:
    rr::IniFile iniFile;
    fs::path cAPICoreModelsDir;

    RRHandle gRR;
    rr::RoadRunner r;

    path testModelFilePath;

    CAPICoreTest();

    ~CAPICoreTest() override = default;

    std::string getListOfReactionsText(const string &fName);
};


#endif // ROADRUNNER_CAPICORETEST
