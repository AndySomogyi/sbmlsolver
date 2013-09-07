#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop

#include "rrLogger.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrSimulationSettings.h"
#include "rrSelectionRecord.h"

#include <map>
#include <algorithm>

using namespace std;
namespace rr
{

SimulationSettings::SimulationSettings(const std::string &fname)
:
mSteps(50),
mStartTime(0),
mDuration(5),
mEndTime(mStartTime + mDuration),
mAbsolute(1.e-10),
mRelative(1.e-10)
{
    if (!fname.empty())
    {
        loadFromFile(fname);
    }

    mAbsolute = 1.e-12;
    mRelative = 1.e-9;
}

void SimulationSettings::clearSettings()
{
    mSteps      = 0;
    mStartTime  = 0;
    mDuration   = 0;
    mVariables.clear();
    mAmount.clear();
    mConcentration.clear();
}

bool SimulationSettings::loadFromFile(const string& _FName)
{
    string fName(_FName);

    if(!fName.size())
    {
        Log(lError)<<"Empty file name for setings file";
        return false;
    }
    else
    {
        clearSettings();
        map<string, string> settings;
        map<string, string>::iterator it;
        //Read each line in the settings file
        vector<string> lines = getLinesInFile(fName);
        for(int i = 0; i < lines.size(); i++)
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
        mStartTime = (it != settings.end())   ? toDouble((*it).second) : 0;

        it = settings.find("duration");
        mDuration = (it != settings.end())    ? toDouble((*it).second) : 0;

        it = settings.find("steps");
        mSteps = (it != settings.end())       ? toInt((*it).second) : 50;

        it = settings.find("absolute");
        mAbsolute = (it != settings.end())    ? toDouble((*it).second) : 1.e-7;

        it = settings.find("relative");
        mRelative = (it != settings.end())    ? toDouble((*it).second) : 1.e-4;

        mEndTime = mStartTime + mDuration;

        it = settings.find("variables");
        if(it != settings.end())
        {
            vector<string> vars = splitString((*it).second, ",");
            for(int i=0; i < vars.size(); i++)
            {
                mVariables.push_back(trim(vars[i]));
            }
        }

        it = settings.find("amount");
        if(it != settings.end())
        {
            vector<string> vars = splitString((*it).second, ",");
            for(int i=0; i < vars.size(); i++)
            {
                string rec = trim(vars[i]);
                if(rec.size())
                {
                    mAmount.push_back(rec);
                }
            }
        }

        it = settings.find("concentration");
        if(it != settings.end())
        {
            vector<string> vars = splitString((*it).second, ",");
            for(int i=0; i < vars.size(); i++)
            {
                string rec = trim(vars[i]);
                if(rec.size())
                {
                    mConcentration.push_back(rec);
                }
            }
        }
    }

    return true;
}


std::vector<std::string> SimulationSettings::getSelectionList() const
{
    //read from settings the variables found in the amounts and concentrations lists
    std::vector<std::string> theList;
    SelectionRecord record;

    theList.push_back("time");

    int nrOfVars = mVariables.size();

    for(int i = 0; i < mAmount.size(); i++)
    {
        theList.push_back("[" + mAmount[i] + "]");        //In the setSelection list below, the [] selects the correct 'type'
    }

    for(int i = 0; i < mConcentration.size(); i++)
    {
        theList.push_back(mConcentration[i]);
    }

    //We may have variables
    //A variable 'exists' only in "variables", not in the amount or concentration section

    for(int i = 0; i < mVariables.size(); i++)
    {
        string aVar = mVariables[i];

        if ((find(mAmount.begin(), mAmount.end(), aVar) == mAmount.end()) &&
                (find(mConcentration.begin(), mConcentration.end(), aVar)
                        == mConcentration.end()))
        {
            theList.push_back(mVariables[i]);
        }
    }

    return theList;
}



}
