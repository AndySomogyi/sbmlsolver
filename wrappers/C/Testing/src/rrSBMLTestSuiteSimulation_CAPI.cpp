#pragma hdrstop
#include "rrException.h"
#include "rrUtils.h"
#include "rrRoadRunnerData.h"
#include "rrSBMLTestSuiteSimulation_CAPI.h"
#include "rrRoadRunner.h"
#include "rrLogger.h"

using namespace rrc;
using namespace std;

extern string gTempFolder;
extern string gTSModelsPath;
extern string gCompiler;


using namespace rr;
RoadRunnerData convertCAPIResultData(RRCDataPtr        resultsHandle);

SBMLTestSuiteSimulation_CAPI::SBMLTestSuiteSimulation_CAPI(const string& dataOutputFolder, const string& modelFilePath, const string& modelFileName)
:
        rr::TestSuiteModelSimulation(dataOutputFolder, modelFilePath, modelFileName),
        mRRHandle(NULL),
        mResultHandle(NULL)
{
}

SBMLTestSuiteSimulation_CAPI::~SBMLTestSuiteSimulation_CAPI()
{
    // we own the result, but not the RR instance.
    if(mResultHandle)
    {
        freeRRCData(mResultHandle);
    }
}

void SBMLTestSuiteSimulation_CAPI::UseHandle(RRHandle handle)
{
    mRRHandle = handle;
    if(mRRHandle)
    {
        this->UseEngine((RoadRunner*) mRRHandle);
    }
}


bool SBMLTestSuiteSimulation_CAPI::LoadSBMLFromFile()
{
    if(!mRRHandle)
    {
        return false;
    }

    try
    {
        std::string fileName = GetModelsFullFilePath();

        //Check first if file exists first
        if(!fileExists(fileName))
        {
            Log(Logger::LOG_ERROR) << "sbml file " << fileName << " not found";
            return false;
        }

        RoadRunner* rri = (RoadRunner*) mRRHandle;

        LoadSBMLOptions opt;

        // don't generate cache for models
        opt.modelGeneratorOpt = opt.modelGeneratorOpt | LoadSBMLOptions::RECOMPILE;

        // no mutable initial conditions
        opt.modelGeneratorOpt = opt.modelGeneratorOpt & ~LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS;

        // read only model
        opt.modelGeneratorOpt = opt.modelGeneratorOpt | LoadSBMLOptions::READ_ONLY;

        opt.modelGeneratorOpt = opt.modelGeneratorOpt | LoadSBMLOptions::OPTIMIZE_CFG_SIMPLIFICATION;

        opt.modelGeneratorOpt = opt.modelGeneratorOpt | LoadSBMLOptions::OPTIMIZE_GVN;


        rri->load(fileName, &opt);
        return true;
    }
    catch(std::exception& e)
    {
        Log(Logger::LOG_ERROR) << "exception while loading sbml: " << e.what();
        return false;
    }
}

bool SBMLTestSuiteSimulation_CAPI::LoadSettings(const string& settingsFName)
{
    mModelSettingsFileName = (settingsFName);

    if(!mModelSettingsFileName.size())
    {
        mModelSettingsFileName = joinPath(mModelFilePath, GetSettingsFileNameForCase(mCurrentCaseNumber));
    }
    SBMLModelSimulation::LoadSettings(mModelSettingsFileName);

    RoadRunner* rri = (RoadRunner*) mRRHandle;

    SimulateOptions options(mModelSettingsFileName.c_str());

    rri->setSimulateOptions(options);

    return true;
}

bool SBMLTestSuiteSimulation_CAPI::Simulate()
{
    if(!mRRHandle)
    {
        return false;
    }

    //Todo: Bisare
    simulate(mRRHandle);
    mResultHandle = getSimulationResult(mRRHandle);

    if(mResultHandle)
    {
        mResultData = convertCAPIResultData(mResultHandle);
    }

    return mResultHandle ? true : false;
}

RoadRunnerData SBMLTestSuiteSimulation_CAPI::GetResult()
{
    return mResultData; //Not that pretty.
}

bool SBMLTestSuiteSimulation_CAPI::SaveResult()
{
    string resultFileName(joinPath(mDataOutputFolder, "rrCAPI_" + mModelFileName));
    resultFileName = changeFileExtensionTo(resultFileName, ".csv");

    if(!mResultHandle)
    {
        return false;
    }

    ofstream fs(resultFileName.c_str());
    fs << mResultData;
    fs.close();
    return true;
}

RoadRunnerData convertCAPIResultData(RRCDataPtr    result)
{
    RoadRunnerData resultData;

    vector<string> colNames;
    //Copy column names
    for(int i = 0; i < result->CSize; i++)
    {
        colNames.push_back(result->ColumnHeaders[i]);
    }

    resultData.setColumnNames(colNames);

    //Then the data
    int index = 0;
    resultData.allocate(result->RSize, result->CSize);

    for(int j = 0; j < result->RSize; j++)
    {
        for(int i = 0; i < result->CSize; i++)
        {
            resultData(j,i) = result->Data[index++];
        }
    }

    return resultData;
}


bool RunTest(const string& version, int caseNumber)
{
    bool result(false);
    RRHandle gRR;

    //Create instance..
    gRR = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());


    //Setup environment
    setTempFolder(gRR, gTempFolder.c_str());

    if(!gRR)
    {
        return false;
    }

    try
    {
        Log(Logger::LOG_NOTICE) << "Running Test: "<< caseNumber << endl;
        string dataOutputFolder(gTempFolder);
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
            throw(rr::Exception(msg));
        }

        SBMLTestSuiteSimulation_CAPI simulation(dataOutputFolder);

        simulation.UseHandle(gRR);

        //Read SBML models.....
        string modelFilePath(gTSModelsPath);
        string modelFileName;

        simulation.SetCaseNumber(caseNumber);
        createTestSuiteFileNameParts(caseNumber, "-sbml-" + version + ".xml", modelFilePath, modelFileName, settingsFileName);

        //The following will load and compile and simulate the sbml model in the file
        simulation.SetModelFilePath(modelFilePath);
        simulation.SetModelFileName(modelFileName);
        simulation.ReCompileIfDllExists(true);
        simulation.CopyFilesToOutputFolder();
        setTempFolder(gRR, simulation.GetDataOutputFolder().c_str());
        setComputeAndAssignConservationLaws(gRR, false);

        if(!simulation.LoadSBMLFromFile())
        {
            throw(Exception("Failed loading sbml from file"));
        }

        //Then read settings file if it exists..
        string settingsOveride("");
        if(!simulation.LoadSettings(settingsOveride))
        {
            throw(Exception("Failed loading simulation settings"));
        }

        //Then Simulate model
        if(!simulation.Simulate())
        {
            throw(Exception("Failed running simulation"));
        }

        //Write result
        if(!simulation.SaveResult())
        {
            //Failed to save data
            throw(Exception("Failed saving result"));
        }

        if(!simulation.LoadReferenceData())
        {
            throw(Exception("Failed Loading reference data"));
        }

        simulation.CreateErrorData();
        result = simulation.Pass();
        simulation.SaveAllData();
        simulation.SaveModelAsXML(dataOutputFolder);
        if(!result)
        {
            clog<<"\t\t =============== Test "<<caseNumber<<" failed =============\n";
        }
        else
        {
            clog<<"\t\tTest passed..\n";
        }
    }
    catch(std::exception& ex)
    {
        string error = ex.what();
        cerr<<"Case "<<caseNumber<<": Exception: "<<error<<endl;
        freeRRInstance(gRR);
        return false;
    }

    freeRRInstance(gRR);
    return result;
}
