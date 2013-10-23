/*
 * TestRoadRunner.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "TestRoadRunner.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLogger.h"
#include "rrc_api.h"
#include "rrSBMLReader.h"

using namespace std;

namespace rr
{


TestRoadRunner::~TestRoadRunner()
{
    delete simulation;
    delete rr;
}

TestRoadRunner::TestRoadRunner(const std::string& version, int caseNumber) :
        version(version), caseNumber(caseNumber), rr(0), simulation(0)
{
    //fileName = getModelFileName(version, caseNumber);

    home = getenv("HOME");
    dataOutputFolder = home + string("/tmp");
    string dummy;
    string logFileName;
    string settingsFileName;



    //Create a log file name
    createTestSuiteFileNameParts(caseNumber, ".log", dummy, logFileName, settingsFileName);

    //Create subfolder for data output
    dataOutputFolder = joinPath(dataOutputFolder, getTestSuiteSubFolderName(caseNumber));

    if(!createFolder(dataOutputFolder))
    {
        string msg("Failed creating output folder for data output: " + dataOutputFolder);
        throw(Exception(msg));
    }
}

bool TestRoadRunner::test(const std::string& compiler)
{
    loadSBML(compiler);

    simulate();

    return true;
}

void TestRoadRunner::loadSBML(const std::string& compiler)
{
    rr = new RoadRunner(compiler, dataOutputFolder, home + "/local/rr_support");

    simulation = new TestSuiteModelSimulation(dataOutputFolder);

    simulation->UseEngine(rr);

    //Read SBML models.....
    modelFilePath = home + "/src/sbml_test/cases/semantic";

    simulation->SetCaseNumber(caseNumber);
    createTestSuiteFileNameParts(caseNumber, "-sbml-" + version + ".xml",
            modelFilePath, modelFileName, settingsFileName);

    //The following will load and compile and simulate the sbml model in the file
    simulation->SetModelFilePath(modelFilePath);
    simulation->SetModelFileName(modelFileName);
    simulation->ReCompileIfDllExists(true);
    simulation->CopyFilesToOutputFolder();
    //setTempFolder(gRR, simulation.GetDataOutputFolder().c_str());
    //setComputeAndAssignConservationLaws(gRR, false);

    //rr->loadSBMLFromFile(fileName);

    if (!simulation->LoadSBMLFromFile())
    {
        throw Exception("Failed loading sbml from file");
    }

    //Then read settings file if it exists..
    string settingsOveride("");
    if (!simulation->LoadSettingsEx(settingsOveride))
    {
        Log(lError) << "Failed loading SBML model settings";
        throw Exception("Failed loading SBML model settings");
    }

    rr->setConservationAnalysis(false);
}

void TestRoadRunner::simulate()
{
    SimulateOptions options;
    options.start = rr->getSimulateOptions().start;
    options.duration = rr->getSimulateOptions().duration;
    options.steps = rr->getSimulateOptions().steps;
    options.flags = options.flags | SimulateOptions::RESET_MODEL;

    if (!rr->simulate(&options))
    {
        throw Exception("Simulation Failed");
    }
}

void TestRoadRunner::saveResult()
{
    if (!simulation->SaveResult())
    {
        //Failed to save data
        throw Exception("Failed saving result");
    }
}

void TestRoadRunner::compareReference()
{
    if (!simulation->LoadReferenceData())
    {
        throw Exception("Failed Loading reference data");
    }

    simulation->CreateErrorData();
    bool result = simulation->Pass();
    simulation->SaveAllData();
    simulation->SaveModelAsXML(dataOutputFolder);
    if (!result)
    {
        Log(Logger::PRIO_NOTICE) << "Test failed..\n";
    }
    else
    {
        Log(Logger::PRIO_NOTICE) << "Test passed..\n";
    }
}

void TestRoadRunner::test2()
{
#ifndef _WIN32
    rrc::RRHandle rr = rrc::createRRInstance();

    rrc::loadSBMLFromFile(rr, "/home/andy/src/sbml_test/cases/semantic/00001/00001-sbml-l3v1.xml");

    //rrc::setTimeStart(rr, 0);
    //rrc::setTimeEnd(rr, 20);
    //rrc::setNumPoints(rr, 100);

    rrc::RRCDataPtr data = rrc::simulateEx(rr, 0, 20, 100);

    std::cout << "columns: " << data->CSize << ", rows: " << data->RSize << std::endl;

    for (int r = 0; r < data->RSize; ++r) {
        cout << "row " << r << ", [";
        for (int c = 0; c < data->CSize; ++c) {
            cout << data->Data[r*data->CSize + c] << ", ";
        }
        cout << "]" << std::endl;
    }
#endif
}


void TestRoadRunner::test3()
{
#ifndef _WIN32
    rrc::RRHandle rr = rrc::createRRInstance();

    rrc::loadSBMLFromFile(rr, "/Users/andy/Desktop/Feedback.xml");

    rrc::setTimeStart(rr, 0);
    rrc::setTimeEnd(rr, 20);
    rrc::setNumPoints(rr, 400);

    rrc::RRCDataPtr data = rrc::simulate(rr);

    std::cout << "columns: " << data->CSize << ", rows: " << data->RSize << std::endl;

    for (int r = 0; r < data->RSize; ++r) {
        cout << "row " << r << ", [";
        for (int c = 0; c < data->CSize; ++c) {
            cout << data->Data[r*data->CSize + c] << ", ";
        }
        cout << "]" << std::endl;
    }
#endif
}



/*

bool RunTest(const string& version, int caseNumber)
{
    bool result = false;
    RRHandle gRR = 0;

    //Create instance..
    gRR = createRRInstanceEx(gTempFolder.c_str());

    if(gDebug && gRR)
    {
        enableLoggingToConsole();
        setLogLevel("Debug5");
    }
    else
    {
        setLogLevel("Error");
    }

    //Setup environment
    setTempFolder(gRR, gTempFolder.c_str());

    if(!gRR)
    {
        return false;
    }

    try
    {




        //Then read settings file if it exists..
        string settingsOveride("");
        if(!simulation.LoadSettings(settingsOveride))
        {
            throw("Failed loading simulation settings");
        }

        //Then Simulate model
        if(!simulation.Simulate())
        {
            throw("Failed running simulation");
        }

        //Write result
        if(!simulation.SaveResult())
        {
            //Failed to save data
            throw("Failed saving result");
        }

        if(!simulation.LoadReferenceData())
        {
            throw("Failed Loading reference data");
        }

        simulation.CreateErrorData();
        result = simulation.Pass();
        simulation.SaveAllData();
        simulation.SaveModelAsXML(dataOutputFolder);
        if(!result)
        {
            clog<<"\t\tTest failed..\n";
        }
        else
        {
            clog<<"\t\tTest passed..\n";
        }
    }
    catch(rr::Exception& ex)
    {
        string error = ex.what();
        cerr<<"Case "<<caseNumber<<": Exception: "<<error<<endl;
        result = false;;
    }

    // done with rr
    freeRRInstance(gRR);
    return result;
}

 */

SelectionRecord TestRoadRunner::testsel(const std::string& str)
{
    return SelectionRecord(str);
}

std::string TestRoadRunner::read_uri(const std::string& uri)
{

    return SBMLReader::read(uri);

/*


    try
    {
        Poco::Net::HTTPStreamFactory::registerFactory();

        Poco::URIStreamOpener &opener = Poco::URIStreamOpener::defaultOpener();


        std::istream* stream = opener.open(uri);

        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(*stream), eos);

        return s;

    }
    catch(std::exception& ex)
    {
        cout << "caught exception " << ex.what() << endl;
        return ex.what();
    }

*/
}


void TestRoadRunner::steadyState(const std::string& uri)
{
    Logger::setLevel(Logger::PRIO_DEBUG);
    RoadRunner r;

    r.load(uri);

    r.steadyState();
}

void TestRoadRunner::testLoad(const std::string& uri)
{
    try
    {
    Logger::setLevel(Logger::PRIO_DEBUG);

    //std::string sbml = SBMLReader::read(uri);


    RoadRunner r;

    r.load(uri);

    r.steadyState();
    }
    catch(std::exception& e)
    {
        cout << "error: " << e.what() << std::endl;
    }
}

void TestRoadRunner::testLogging(const std::string& logFileName)
{
    Logger::init("", Logger::PRIO_NOTICE);

    Log(Logger::PRIO_NOTICE) << "notice";

    Log(Logger::PRIO_NOTICE) << "setting logging to file: " << logFileName;

    Logger::init("", Logger::PRIO_NOTICE, logFileName);

    cout << "log file name: " << Logger::getFileName() << endl;

    Log(Logger::PRIO_FATAL) << "A fatal error to file";
    Log(Logger::PRIO_CRITICAL) << "A critical error. to file";
    Log(Logger::PRIO_ERROR) << "An error. to file";
    Log(Logger::PRIO_WARNING) << "A warning. to file";
    Log(Logger::PRIO_NOTICE) << "A notice,to file ";
    Log(Logger::PRIO_INFORMATION) << "An informational message, to file ";
    Log(Logger::PRIO_DEBUG) << "A debugging message. to file";
    Log(Logger::PRIO_TRACE) << "A tracing message. to file";

    Logger::init("", Logger::PRIO_NOTICE);

    cout << "log file name: " << Logger::getFileName() << endl;

    Log(Logger::PRIO_FATAL) << "A fatal error. to cons";
    Log(Logger::PRIO_CRITICAL) << "A critical error. to cons";
    Log(Logger::PRIO_ERROR) << "An error. to cons";
    Log(Logger::PRIO_WARNING) << "A warning. to cons";
    Log(Logger::PRIO_NOTICE) << "A notice, to cons";
    Log(Logger::PRIO_INFORMATION) << "An informational message, to cons";
    Log(Logger::PRIO_DEBUG) << "A debugging message.to cons";
    Log(Logger::PRIO_TRACE) << "A tracing message. to cons";
}

void TestRoadRunner::test_fs75()
{
    const char* src = "/Users/andy/fs75.xml";

    rrc::RRHandle r = rrc::createRRInstance();

    rrc::loadSBMLFromFile(r, src);

    double conc[] = {1.0, 2.0, 3.0};

    rrc::RRVector concVec;

    concVec.Count = 3;
    concVec.Data = conc;

    rrc::RRVectorPtr res = rrc::getReactionRatesEx(r, &concVec);

    for (int i = 0; i < res->Count; ++i)
    {
        cout << "index " << i << ": " << res->Data[i] << endl;
    }
}

void TestRoadRunner::test_fs74()
{
    rrc::RRHandle r = rrc::createRRInstance();

    rrc::loadSBMLFromFile(r, "");

    cout << "error:: " << rrc::getLastError() << endl;

}

void TestRoadRunner::test_fs73()
{
    const char* src = "/Users/andy/fs75.xml";

    rrc::RRHandle r = rrc::createRRInstance();

    rrc::loadSBMLFromFile(r, src);

    rrc::setFloatingSpeciesByIndex(r, 1, 3.14);

    double result = 0;

    rrc::getFloatingSpeciesByIndex(r, 1, &result);

    cout << "result: " << result << endl;
}

} /* namespace rr */


