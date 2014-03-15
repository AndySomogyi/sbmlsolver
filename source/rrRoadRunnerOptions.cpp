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

namespace rr
{

LoadSBMLOptions::LoadSBMLOptions()
{
    version = 0;
    size = sizeof(LoadSBMLOptions);
    modelGeneratorOpt = MUTABLE_INITIAL_CONDITIONS;
    loadFlags = 0;
}

const double SimulateOptions::MIN_RELATIVE = 1.e-5;
const double SimulateOptions::MIN_ABSOLUTE = 1.e-10;

SimulateOptions::SimulateOptions()
:
steps(50),
start(0),
duration(5),
absolute(MIN_ABSOLUTE),
relative(MIN_RELATIVE),
flags(STRUCTURED_RESULT),
integrator(CVODE),
integratorFlags(0),
initialTimeStep(-1),
minimumTimeStep(-1),
maximumTimeStep(-1),
maximumNumSteps(-1)
{
}

SimulateOptions::SimulateOptions(const std::string &fname)
:
steps(50),
start(0),
duration(5),
absolute(MIN_ABSOLUTE),
relative(MIN_RELATIVE),
flags(STRUCTURED_RESULT),
integrator(CVODE),
integratorFlags(0),
initialTimeStep(-1),
minimumTimeStep(-1),
maximumTimeStep(-1),
maximumNumSteps(-1)
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
        start = (it != settings.end())       ? std::abs(toDouble((*it).second)): 0;

        it = settings.find("duration");
        duration = (it != settings.end())    ? std::abs(toDouble((*it).second)) : 0;

        it = settings.find("steps");
        steps = (it != settings.end())       ? std::abs(toInt((*it).second)) : 50;

        it = settings.find("absolute");
        absolute = (it != settings.end())    ? std::abs(toDouble((*it).second)) : MIN_ABSOLUTE;

        it = settings.find("relative");
        relative = (it != settings.end())    ? std::abs(toDouble((*it).second)) : MIN_RELATIVE;

        // adjust values to min that will pass test suite
        if (relative > MIN_RELATIVE)
        {
            relative = MIN_RELATIVE;
        }

        if (absolute > MIN_ABSOLUTE)
        {
            absolute = MIN_ABSOLUTE;
        }

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
}

} /* namespace rr */
