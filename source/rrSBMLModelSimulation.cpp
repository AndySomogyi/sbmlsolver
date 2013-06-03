#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iomanip>
#include <map>
#include "rrLogger.h"
#include "rrSBMLModelSimulation.h"
#include "rrUtils.h"
#include "rrRoadRunner.h"
//---------------------------------------------------------------------------

namespace rr
{

SBMLModelSimulation::SBMLModelSimulation(const string& dataOutputFolder, const string& tempDataFilePath)
:
mModelFilePath(""),
mModelFileName(""),
mDataOutputFolder(dataOutputFolder),
mCompileIfDllExists(true),
mTempDataFolder(tempDataFilePath),
mEngine(NULL)
{
    mSettings.mAbsolute    = 1.e-7;
    mSettings.mRelative    = 1.e-4;
}

SBMLModelSimulation::~SBMLModelSimulation()
{}

string SBMLModelSimulation::GetTempDataFolder()
{
    return mTempDataFolder;
}

void SBMLModelSimulation::ReCompileIfDllExists(const bool& doIt)
{
    mCompileIfDllExists = doIt;
}

bool SBMLModelSimulation::SetModelFilePath(const string& path)
{
    mModelFilePath = path;
    return true;
}

RoadRunnerData SBMLModelSimulation::GetResult()
{
    if(mEngine)
    {
        return mEngine->getSimulationResult();
    }
    else
    {
        return RoadRunnerData();
    }
}

bool SBMLModelSimulation::SetModelFileName(const string& name)
{
    if(getFilePath(name).size() > 0)
    {
        mModelFilePath = getFilePath(name);
    }

    mModelFileName = getFileName(name);

    if(!fileExists(joinPath(mModelFilePath, mModelFileName)))
    {
        Log(lError)<<"The file: "<<joinPath(mModelFilePath, mModelFileName)<<" don't exist.";
        return false;
    }

    return true;
}

bool SBMLModelSimulation::SetDataOutputFolder(const string& name)
{
    mDataOutputFolder = name;
    return true;
}

string  SBMLModelSimulation::GetModelsFullFilePath()
{
    return joinPath(mModelFilePath, mModelFileName);
}

string  SBMLModelSimulation::GetDataOutputFolder()
{
    return mDataOutputFolder;
}

bool SBMLModelSimulation::DoCompileIfDllExists()
{
    return mCompileIfDllExists;
}

bool SBMLModelSimulation::UseEngine(RoadRunner* engine)
{
    mEngine = engine;
    if(mEngine)
    {
        mEngine->partOfSimulation(this);    //Road runner then gets access to data oupt folders etc..
    }
    return true;
}

bool SBMLModelSimulation::GenerateModelCode()
{
    return true;
}

bool SBMLModelSimulation::CompileModel()
{
    return true;
}

bool SBMLModelSimulation::LoadSettings(const string& settingsFName)
{
    string fName(settingsFName);

    if(!fName.size())
    {
        Log(lError)<<"Empty file name for setings file";
        return false;
    }
    else
    {
        map<string, string> settings;
        map<string, string>::iterator it;
        //Read each line in the settings file
        vector<string> lines = getLinesInFile(fName);
        for(u_int i = 0; i < lines.size(); i++)
        {
            vector<string> line = splitString(lines[i], ":");
            if(line.size() == 2)
            {
                settings.insert( pair<string, string>(line[0], line[1]));
            }
            else
            {
                Log(lDebug2)<<"Empty line in settings file: "<<lines[i];
            }
        }

        Log(lDebug3)<<"Settings File =============";
        for (it = settings.begin() ; it != settings.end(); it++ )
        {
            Log(lDebug) << (*it).first << " => " << (*it).second;
        }
        Log(lDebug)<<"===========================";

        //Assign values
        it = settings.find("start");
        mSettings.mStartTime = (it != settings.end())   ? toDouble((*it).second) : 0;

        it = settings.find("duration");
        mSettings.mDuration = (it != settings.end())    ? toDouble((*it).second) : 0;

        it = settings.find("steps");
        mSettings.mSteps = (it != settings.end())       ? toInt((*it).second) : 50;

        it = settings.find("absolute");
        mSettings.mAbsolute = (it != settings.end())    ? toDouble((*it).second) : 1.e-7;

        it = settings.find("relative");
        mSettings.mRelative = (it != settings.end())    ? toDouble((*it).second) : 1.e-4;

        mSettings.mEndTime = mSettings.mStartTime + mSettings.mDuration;

        it = settings.find("variables");
        if(it != settings.end())
        {
            vector<string> vars = splitString((*it).second, ",");
            for(u_int i = 0; i < vars.size(); i++)
            {
                mSettings.mVariables.add(trim(vars[i]));
            }
        }

        it = settings.find("amount");
        if(it != settings.end())
        {
            vector<string> vars = splitString((*it).second, ",");
            for(u_int i = 0; i < vars.size(); i++)
            {
                string rec = trim(vars[i]);
                if(rec.size())
                {
                    mSettings.mAmount.add(rec);
                }
            }
        }

        it = settings.find("concentration");
        if(it != settings.end())
        {
            vector<string> vars = splitString((*it).second, ",");
            for(u_int i=0; i < vars.size(); i++)
            {
                string rec = trim(vars[i]);
                if(rec.size())
                {
                    mSettings.mConcentration.add(rec);
                }
            }
        }
    }

    if(mEngine)
    {
        mEngine->useSimulationSettings(mSettings);

        //This one creates the list of what we will look at in the result
        mEngine->createTimeCourseSelectionList();
    }

    return true;
}

bool SBMLModelSimulation::SetTimeStart(const double& startTime)
{
    mSettings.mStartTime   = startTime;
    return true;
}

bool SBMLModelSimulation::SetTimeEnd(const double& endTime)
{
    mSettings.mEndTime = endTime;
    mSettings.mDuration = mSettings.mEndTime - mSettings.mStartTime;
    return true;
}

bool SBMLModelSimulation::SetNumberOfPoints(const int& steps)
{
    mSettings.mSteps       = steps;
    return true;
}

bool SBMLModelSimulation::SetSelectionList(const string& selectionList)
{
    vector<string> vars = splitString(selectionList, ", ");
    for(u_int i = 0; i < vars.size(); i++)
    {
        mSettings.mVariables.add(trim(vars[i]));
    }

    mEngine->useSimulationSettings(mSettings);
    mEngine->createTimeCourseSelectionList();    //This one creates the list of what we will look at in the result
    return true;
}


bool SBMLModelSimulation::LoadSBMLFromFile()                    //Use current file information to load sbml from file
{
    if(!mEngine)
    {
        return false;
    }

    bool val = mEngine->loadSBMLFromFile(GetModelsFullFilePath(), mCompileIfDllExists);
    return val;
}

bool SBMLModelSimulation::SaveModelAsXML(const string& folder)
{
    if(!mEngine)
    {
        return false;
    }
    string fName = joinPath(folder, mModelFileName);
    fName = changeFileExtensionTo(fName, "xml");

    fstream fs(fName.c_str(), fstream::out);

    if(!fs)
    {
        Log(lError)<<"Failed writing sbml to file "<< fName;
        return false;
    }
    fs<<mEngine->getSBML();
    fs.close();
    return true;
}

bool SBMLModelSimulation::CreateModel()
{
    if(!mEngine)
    {
        return false;
    }

    return true;
}

bool SBMLModelSimulation::InitializeModel()
{
    if(!mEngine)
    {
        return false;
    }

    return mEngine->initializeModel();
}

bool SBMLModelSimulation::GenerateAndCompileModel()
{
    return true;
}

bool SBMLModelSimulation::Simulate()
{
    if(!mEngine)
    {
        return false;
    }

    return mEngine->simulate2();
}

bool SBMLModelSimulation::SaveResult()
{
    string resultFileName(joinPath(mDataOutputFolder, "rr_" + mModelFileName));
    resultFileName = changeFileExtensionTo(resultFileName, ".csv");
    Log(lInfo)<<"Saving result to file: "<<resultFileName;
    RoadRunnerData resultData = mEngine->getSimulationResult();

    ofstream fs(resultFileName.c_str());
    fs << resultData;
    fs.close();
    return true;
}
} //end of namespace


