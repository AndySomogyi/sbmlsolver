/*
 * rrRoadRunnerOptions.cpp
 *
 *  Created on: Sep 9, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "rrRoadRunnerOptions.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrSelectionRecord.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

static const double MaxRelative = 1.e-9;
static const double MaxAbsolute = 1.e-11;

namespace rr
{

LoadSBMLOptions::LoadSBMLOptions()
{
    version = 0;
    size = sizeof(LoadSBMLOptions);
    modelGeneratorOpt = MUTABLE_INITIAL_CONDITIONS;
    loadFlags = 0;
}

SimulateOptions::SimulateOptions()
:
steps(50),
start(0),
duration(5),
absolute(MaxAbsolute),
relative(MaxRelative),
flags(STRUCTURED_RESULT),
integrator(CVODE),
integratorFlags(0)
{
}

SimulateOptions::SimulateOptions(const std::string &fname)
:
steps(50),
start(0),
duration(5),
absolute(MaxAbsolute),
relative(MaxRelative),
flags(STRUCTURED_RESULT),
integrator(CVODE),
integratorFlags(0)
{

    if(!fname.size())
    {
        Log(Logger::LOG_ERROR)<<"Empty file name for setings file";
    }
    else
    {
        map<string, string> settings;
        map<string, string>::iterator it;
        //Read each line in the settings file
        vector<string> lines = getLinesInFile(fname);
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
        start = (it != settings.end())   ? toDouble((*it).second) : 0;

        it = settings.find("duration");
        duration = (it != settings.end())    ? toDouble((*it).second) : 0;

        it = settings.find("steps");
        steps = (it != settings.end())       ? toInt((*it).second) : 50;

        it = settings.find("absolute");
        absolute = (it != settings.end())    ? toDouble((*it).second) : 1.e-7;

        it = settings.find("relative");
        relative = (it != settings.end())    ? toDouble((*it).second) : 1.e-4;

        it = settings.find("variables");
        if(it != settings.end())
        {
            vector<string> vars = splitString((*it).second, ",");
            for(int i=0; i < vars.size(); i++)
            {
                variables.push_back(trim(vars[i]));
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
                    amounts.push_back(rec);
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
                    concentrations.push_back(rec);
                }
            }
        }
    }

    if (absolute > MaxAbsolute) {
        absolute = MaxAbsolute;
    }

    if (relative > MaxRelative) {
        relative = MaxRelative;
    }
}

} /* namespace rr */
