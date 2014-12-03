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
#include "rrConfig.h"

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <assert.h>

using namespace std;

namespace rr
{

static const char* keys[] = {
        "integrator",
        "steps",
        "start",
        "duration",
        "absolute",
        "relative",
        "initialTimeStep",
        "minimumTimeStep",
        "maximumTimeStep",
        "maximumNumSteps",
        "reset",
        "copyResult",
        "stiff",
        "multiStep",
        "variableStep",

        "steps.description",
        "start.description",
        "duration.description",
        "absolute.description",
        "relative.description",
        "initialTimeStep.description",
        "minimumTimeStep.description",
        "maximumTimeStep.description",
        "maximumNumSteps.description",
        "reset.description",
        "copyResult.description",
        "stiff.description",
        "multiStep.description",
        "variableStep.description",

        "steps.hint",
        "start.hint",
        "duration.hint",
        "absolute.hint",
        "relative.hint",
        "initialTimeStep.hint",
        "minimumTimeStep.hint",
        "maximumTimeStep.hint",
        "maximumNumSteps.hint",
        "reset.hint",
        "copyResult.hint",
        "stiff.hint",
        "multiStep.hint",
        "variableStep.hint"
};

static const char* descriptions[] = {
        "steps.description",
        "start.description",
        "duration.description",
        "absolute.description",
        "relative.description",
        "initialTimeStep.description",
        "minimumTimeStep.description",
        "maximumTimeStep.description",
        "maximumNumSteps.description",
        "reset.description",
        "copyResult.description",
        "stiff.description",
        "multiStep.description",
        "variableStep.description"
};

static const char* hints[] = {
        "steps.hint",
        "start.hint",
        "duration.hint",
        "absolute.hint",
        "relative.hint",
        "initialTimeStep.hint",
        "minimumTimeStep.hint",
        "maximumTimeStep.hint",
        "maximumNumSteps.hint",
        "reset.hint",
        "copyResult.hint",
        "stiff.hint",
        "multiStep.hint",
        "variableStep.hint"
};

enum key_index {
    KEY_INTEGRATOR,
    KEY_STEPS,
    KEY_START,
    KEY_DURATION,
    KEY_ABSOLUTE,
    KEY_RELATIVE,
    KEY_INITIALTIMESTEP,
    KEY_MINIMUMTIMESTEP,
    KEY_MAXIMUMTIMESTEP,
    KEY_MAXIMUMNUMSTEPS,
    KEY_RESET,
    KEY_COPYRESULT,
    KEY_STIFF,
    KEY_MULTISTEP,
    KEY_VARIABLESTEP,

    KEY_STEPS_DESCRIPTION,
    KEY_START_DESCRIPTION,
    KEY_DURATION_DESCRIPTION,
    KEY_ABSOLUTE_DESCRIPTION,
    KEY_RELATIVE_DESCRIPTION,
    KEY_INITIALTIMESTEP_DESCRIPTION,
    KEY_MINIMUMTIMESTEP_DESCRIPTION,
    KEY_MAXIMUMTIMESTEP_DESCRIPTION,
    KEY_MAXIMUMNUMSTEPS_DESCRIPTION,
    KEY_RESET_DESCRIPTION,
    KEY_COPYRESULT_DESCRIPTION,
    KEY_STIFF_DESCRIPTION,
    KEY_MULTISTEP_DESCRIPTION,
    KEY_VARIABLESTEP_DESCRIPTION,

    KEY_STEPS_HINT,
    KEY_START_HINT,
    KEY_DURATION_HINT,
    KEY_ABSOLUTE_HINT,
    KEY_RELATIVE_HINT,
    KEY_INITIALTIMESTEP_HINT,
    KEY_MINIMUMTIMESTEP_HINT,
    KEY_MAXIMUMTIMESTEP_HINT,
    KEY_MAXIMUMNUMSTEPS_HINT,
    KEY_RESET_HINT,
    KEY_COPYRESULT_HINT,
    KEY_STIFF_HINT,
    KEY_MULTISTEP_HINT,
    KEY_VARIABLESTEP_HINT,
    KEY_END
};

static bool isDescription(key_index index) {
    return index >= KEY_STEPS_DESCRIPTION
            && index <= KEY_VARIABLESTEP_DESCRIPTION;
}

static const char* getDescription(key_index index) {
    assert(sizeof(descriptions)/sizeof(char*)
            == KEY_VARIABLESTEP_DESCRIPTION - KEY_STEPS_DESCRIPTION + 1);
    return descriptions[index - KEY_STEPS_DESCRIPTION];
}

static bool isHint(key_index index) {
    return index >= KEY_STEPS_HINT
            && index <= KEY_VARIABLESTEP_HINT;
}

static const char* getHint(key_index index) {
    assert(sizeof(hints)/sizeof(char*)
            == KEY_VARIABLESTEP_HINT - KEY_STEPS_HINT + 1);
    return hints[index - KEY_STEPS_HINT];
}

static key_index indexFromString(const string& str)
{
    assert(sizeof(keys)/sizeof(char*) == KEY_END);

    for(int i = 0; i < KEY_END; ++i) {
        if (rr::compareNoCase(str, keys[i]) == 0) {
            return (key_index)i;
        }
    }

    return KEY_END;
}

static Variant itemFromIndex(const SimulateOptions& opt, key_index index)
{
    if (isHint(index)) {
        return Variant(getHint(index));
    }

    if (isDescription(index)) {
        return Variant(getDescription(index));
    }

    switch(index) {
    case KEY_INTEGRATOR:
        return IntegratorFactory::getIntegratorNameFromId(opt.integrator);

    case KEY_STEPS:
        return opt.steps;

    case KEY_START:
        return opt.start;

    case KEY_DURATION:
        return opt.duration;

    case KEY_ABSOLUTE:
        return opt.absolute;

    case KEY_RELATIVE:
        return opt.relative;

    case KEY_INITIALTIMESTEP:
        return opt.initialTimeStep;

    case KEY_MINIMUMTIMESTEP:
        return opt.minimumTimeStep;

    case KEY_MAXIMUMTIMESTEP:
        return opt.maximumTimeStep;

    case KEY_MAXIMUMNUMSTEPS:
        return opt.maximumNumSteps;

    case KEY_RESET:
        return (opt.flags & SimulateOptions::RESET_MODEL) != 0;

    case KEY_COPYRESULT:
        return (opt.flags & SimulateOptions::COPY_RESULT) != 0;

    case KEY_STIFF:
        return (opt.integratorFlags & Integrator::STIFF) != 0;

    case KEY_MULTISTEP:
        return (opt.integratorFlags & Integrator::MULTI_STEP) != 0;

    case KEY_VARIABLESTEP:
        return (opt.integratorFlags & Integrator::VARIABLE_STEP) != 0;

    default:
        throw std::invalid_argument("invalid key index: " + toString(index));

    }
}

static void setItemWithIndex(SimulateOptions& opt, key_index index, const Variant& value)
{
    if (isHint(index)) {
        throw std::invalid_argument("hints are read-only");
    }

    if (isDescription(index)) {
        throw std::invalid_argument("descriptions are read-only");
    }

    switch(index) {
    case KEY_INTEGRATOR:
        opt.setIntegrator(IntegratorFactory::getIntegratorIdFromName(value));
        break;

    case KEY_STEPS:
        opt.steps = value;
        break;

    case KEY_START:
        opt.start = value;
        break;

    case KEY_DURATION:
        opt.duration = value;
        break;

    case KEY_ABSOLUTE:
        opt.absolute = value;
        break;

    case KEY_RELATIVE:
        opt.relative = value;
        break;

    case KEY_INITIALTIMESTEP:
        opt.initialTimeStep = value;
        break;

    case KEY_MINIMUMTIMESTEP:
        opt.minimumTimeStep = value;
        break;

    case KEY_MAXIMUMTIMESTEP:
        opt.maximumTimeStep = value;
        break;

    case KEY_MAXIMUMNUMSTEPS:
        opt.maximumNumSteps = value;
        break;

    case KEY_RESET:
    {
        bool b = value;
        if (b) {
            opt.flags |= SimulateOptions::RESET_MODEL;
        } else {
            opt.flags &= !SimulateOptions::RESET_MODEL;
        }
    }
    break;

    case KEY_COPYRESULT:
    {
        bool b = value;
        if (b) {
            opt.flags |= SimulateOptions::COPY_RESULT;
        } else {
            opt.flags &= !SimulateOptions::COPY_RESULT;
        }
    }
    break;

    case KEY_STIFF:
    {
        bool b = value;
        if (b) {
            opt.integratorFlags |= Integrator::STIFF;
        } else {
            opt.integratorFlags &= !Integrator::STIFF;
        }
    }
    break;


    case KEY_MULTISTEP:
    {
        bool b = value;
        if (b) {
            opt.integratorFlags |= Integrator::MULTI_STEP;
        } else {
            opt.integratorFlags &= !Integrator::MULTI_STEP;
        }
    }
    break;


    case KEY_VARIABLESTEP:
    {
        bool b = value;
        if (b) {
            opt.integratorFlags |= Integrator::VARIABLE_STEP;
        } else {
            opt.integratorFlags &= !Integrator::VARIABLE_STEP;
        }
    }
    break;


    default:
        throw std::invalid_argument("invalid key index: " + toString(index));

    }

}


LoadSBMLOptions::LoadSBMLOptions()
{
    version = 0;
    size = sizeof(LoadSBMLOptions);
    modelGeneratorOpt = 0;

    if (Config::getBool(Config::LOADSBMLOPTIONS_CONSERVED_MOIETIES))
        modelGeneratorOpt |= LoadSBMLOptions::CONSERVED_MOIETIES;

    if (Config::getBool(Config::LOADSBMLOPTIONS_RECOMPILE))
        modelGeneratorOpt |= LoadSBMLOptions::RECOMPILE;

    if (Config::getBool(Config::LOADSBMLOPTIONS_READ_ONLY))
        modelGeneratorOpt |= LoadSBMLOptions::READ_ONLY;

    if (Config::getBool(Config::LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS))
        modelGeneratorOpt |= LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS;

    if (Config::getBool(Config::LOADSBMLOPTIONS_OPTIMIZE_GVN))
        modelGeneratorOpt |= LoadSBMLOptions::OPTIMIZE_GVN;

    if (Config::getInt(Config::LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION))
        modelGeneratorOpt |= LoadSBMLOptions::OPTIMIZE_CFG_SIMPLIFICATION;

    if (Config::getBool(Config::LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING))
        modelGeneratorOpt |= LoadSBMLOptions::OPTIMIZE_INSTRUCTION_COMBINING;

    if (Config::getBool(Config::LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION))
        modelGeneratorOpt |= LoadSBMLOptions::OPTIMIZE_DEAD_INST_ELIMINATION;

    if (Config::getBool(Config::LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION))
        modelGeneratorOpt |= LoadSBMLOptions::OPTIMIZE_DEAD_CODE_ELIMINATION;

    if (Config::getBool(Config::LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER))
        modelGeneratorOpt |= LoadSBMLOptions::OPTIMIZE_INSTRUCTION_SIMPLIFIER;

    if (Config::getBool(Config::LOADSBMLOPTIONS_USE_MCJIT))
        modelGeneratorOpt |= LoadSBMLOptions::USE_MCJIT;

    loadFlags = 0;
}


static void getConfigValues(SimulateOptions *s)
{
    if (Config::getBool(Config::SIMULATEOPTIONS_STRUCTURED_RESULT)) {
        s->flags |=  SimulateOptions::STRUCTURED_RESULT;
    } else {
        s->flags &= ~SimulateOptions::STRUCTURED_RESULT;
    }


    if (Config::getBool(Config::SIMULATEOPTIONS_STIFF)) {
        s->integratorFlags |=  Integrator::STIFF;
    } else {
        s->integratorFlags &= ~Integrator::STIFF;
    }

    if (Config::getBool(Config::SIMULATEOPTIONS_MULTI_STEP)) {
        s->integratorFlags |= Integrator::MULTI_STEP;
    } else {
        s->integratorFlags &= ~Integrator::MULTI_STEP;
    }

    // set the variable step based on if we are using a stochastic or deterministic
    // integrator
    if (Config::getString(Config::SIMULATEOPTIONS_INTEGRATOR) == "CVODE") {
        s->integrator = Integrator::CVODE;
    }
    else if (Config::getString(Config::SIMULATEOPTIONS_INTEGRATOR) == "GILLESPIE") {
        s->integrator = Integrator::GILLESPIE;
    }
    else {
        Log(Logger::LOG_WARNING) << "Invalid integrator specified in configuration: "
                << Config::getString(Config::SIMULATEOPTIONS_INTEGRATOR)
        << std::endl << "Defaulting to CVODE";
        s->integrator = Integrator::CVODE;
    }

    bool vs = false;

    if (IntegratorFactory::getIntegratorType(s->integrator) == Integrator::STOCHASTIC) {
        vs = Config::getBool(rr::Config::SIMULATEOPTIONS_STOCHASTIC_VARIABLE_STEP);
    }

    else if (IntegratorFactory::getIntegratorType(s->integrator) == Integrator::DETERMINISTIC) {
        vs = rr::Config::getBool(rr::Config::SIMULATEOPTIONS_DETERMINISTIC_VARIABLE_STEP);
    }

    if (vs) {
        s->integratorFlags |= Integrator::VARIABLE_STEP;
    } else {
        s->integratorFlags &= ~Integrator::VARIABLE_STEP;
    }

    if (rr::Config::getBool(rr::Config::SIMULATEOPTIONS_COPY_RESULT)) {
        s->flags |= SimulateOptions::COPY_RESULT;
    } else {
        s->flags &= ~SimulateOptions::COPY_RESULT;
    }
}



SimulateOptions::SimulateOptions()
:
steps(Config::getInt(Config::SIMULATEOPTIONS_STEPS)),
start(0),
duration(Config::getDouble(Config::SIMULATEOPTIONS_DURATION)),
absolute(Config::getDouble(Config::SIMULATEOPTIONS_ABSOLUTE)),
relative(Config::getDouble(Config::SIMULATEOPTIONS_RELATIVE)),
flags(0),
integrator(Integrator::CVODE),
integratorFlags(0),
initialTimeStep(Config::getDouble(Config::SIMULATEOPTIONS_INITIAL_TIMESTEP)),
minimumTimeStep(Config::getDouble(Config::SIMULATEOPTIONS_MINIMUM_TIMESTEP)),
maximumTimeStep(Config::getDouble(Config::SIMULATEOPTIONS_MAXIMUM_TIMESTEP)),
maximumNumSteps(Config::getInt(Config::SIMULATEOPTIONS_MAXIMUM_NUM_STEPS))
{
    getConfigValues(this);
}

SimulateOptions::SimulateOptions(const std::string &fname)
:
steps(Config::getInt(Config::SIMULATEOPTIONS_STEPS)),
start(0),
duration(Config::getDouble(Config::SIMULATEOPTIONS_DURATION)),
absolute(Config::getDouble(Config::SIMULATEOPTIONS_ABSOLUTE)),
relative(Config::getDouble(Config::SIMULATEOPTIONS_RELATIVE)),
flags(0),
integrator(Integrator::CVODE),
integratorFlags(0),
initialTimeStep(Config::getDouble(Config::SIMULATEOPTIONS_INITIAL_TIMESTEP)),
minimumTimeStep(Config::getDouble(Config::SIMULATEOPTIONS_MINIMUM_TIMESTEP)),
maximumTimeStep(Config::getDouble(Config::SIMULATEOPTIONS_MAXIMUM_TIMESTEP)),
maximumNumSteps(Config::getInt(Config::SIMULATEOPTIONS_MAXIMUM_NUM_STEPS))
{
    getConfigValues(this);

    if(!fname.size())
    {
        Log(Logger::LOG_ERROR)<<"Empty file name for setings file";
    }
    else
    {
        double minAbs = Config::getDouble(Config::CVODE_MIN_ABSOLUTE);
        double minRel = Config::getDouble(Config::CVODE_MIN_RELATIVE);


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
        absolute = (it != settings.end())    ? std::abs(toDouble((*it).second)) : minAbs;

        it = settings.find("relative");
        relative = (it != settings.end())    ? std::abs(toDouble((*it).second)) : minRel;

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

void SimulateOptions::setItem(const std::string& name,
        const rr::Variant& value)
{
    key_index index = indexFromString(name);

    if(index != KEY_END) {
        return setItemWithIndex(*this, index, value);
    }

    return DictionaryImpl::setItem(name, value);
}

Variant SimulateOptions::getItem(const std::string& name) const
{
    key_index index = indexFromString(name);

    if(index != KEY_END) {
        return itemFromIndex(*this, index);
    }

    return DictionaryImpl::getItem(name);
}


RoadRunnerOptions::RoadRunnerOptions() :
    flags(0)
{
    if (Config::getBool(Config::ROADRUNNER_DISABLE_PYTHON_DYNAMIC_PROPERTIES))
    {
        flags |= RoadRunnerOptions::DISABLE_PYTHON_DYNAMIC_PROPERTIES;
    }

    jacobianStepSize = Config::getDouble(Config::ROADRUNNER_JACOBIAN_STEP_SIZE);
}

bool SimulateOptions::hasKey(const std::string& key) const
{
    if (indexFromString(key) != KEY_END) {
        return true;
    }
    return DictionaryImpl::hasKey(key);
}



std::vector<std::string> SimulateOptions::getKeys() const
{
    // vector of standard keys
    std::vector<std::string> result(&keys[0], &keys[KEY_END]);

    // add custom keys in map
    for (VariantMap::const_iterator i = items.begin(); i != items.end(); ++i) {
        result.push_back(i->first);
    }

    return result;
}

int SimulateOptions::deleteItem(const std::string& key)
{
    return items.erase(key);
}


#define BITFIELD2STR(bf) rr::toString((bool)((bf) ? true : false))


std::string SimulateOptions::toString() const
{
    std::stringstream ss;

    ss << "< roadrunner.SimulateOptions() " << endl << "{ "
            << endl << "'this' : " << (void*)this << ", " << std::endl;

    ss << "integrator: ";
    if (integrator == Integrator::CVODE) {
        ss << "\"cvode\"," << std::endl;
    }

    else if (integrator == Integrator::GILLESPIE ) {
        ss << "\"gillespie\"," << std::endl;
    }

    else {
        ss << "\"unknown\"," << std::endl;
    }

    ss << "'stiff' : " << BITFIELD2STR(integratorFlags & Integrator::STIFF) << "," << std::endl;

    ss << "'multiStep' : " << BITFIELD2STR(integratorFlags & Integrator::MULTI_STEP) << "," <<  std::endl;

    ss << "'variableStep' : " << BITFIELD2STR(integratorFlags & Integrator::VARIABLE_STEP) << "," <<  std::endl;

    ss << "'reset' : " << BITFIELD2STR(flags & RESET_MODEL) << "," <<  std::endl;

    ss << "'structuredResult' : " << BITFIELD2STR(flags & STRUCTURED_RESULT) << "," <<  std::endl;

    ss << "'copyResult' : " << BITFIELD2STR(flags & COPY_RESULT) << "," <<  std::endl;

    ss << "'steps' : " << steps << "," <<  std::endl;

    ss << "'start' : " << start << "," <<  std::endl;

    ss << "'duration' : " << duration << "," <<  std::endl;

    ss << "'relative' : " << relative << "," <<  std::endl;

    ss << "'absolute' : " << absolute << "," <<  std::endl;

    ss << "'initialTimeStep' : " << initialTimeStep << "," <<  std::endl;

    ss << "'minimumTimeStep' : " << minimumTimeStep << "," <<  std::endl;

    ss << "'maximumTimeStep' : " << maximumTimeStep << "," <<  std::endl;

    ss << "'maximumNumSteps' : " << maximumNumSteps;

    std::vector<std::string> keys = getKeys();

    if (keys.size() > 0) {
        ss << "," << std::endl;
    }

    for(std::vector<std::string>::iterator i = keys.begin(); i != keys.end(); ++i)
    {
        ss << "'" << *i << "' : ";
        ss << getItem(*i).toString();

        if (std::distance(i, keys.end()) > 1) {
            ss << ", " << std::endl;
        }
    }

    ss << std::endl << "}>";

    return ss.str();
}



std::string SimulateOptions::toRepr() const
{
    std::stringstream ss;
    ss << "< roadrunner.SimulateOptions() { 'this' : "
            << (void*)this << " }>";
    return ss.str();
}

void SimulateOptions::tweakTolerances()
{
    if (integrator == Integrator::CVODE)
    {
        double minAbs = Config::getDouble(Config::CVODE_MIN_ABSOLUTE);
        double minRel = Config::getDouble(Config::CVODE_MIN_RELATIVE);

        absolute = std::min(absolute, minAbs);
        relative = std::min(relative, minRel);

        Log(Logger::LOG_INFORMATION) << "tweaking CVODE tolerances to abs=" << absolute
                << ", rel=" << relative;
    }
}

std::vector<std::string> SimulateOptions::getSimulateOptionsKeys()
{
    assert(sizeof(keys)/sizeof(char*) == KEY_END);
    const char** begin = &keys[0];
    const char** end = &keys[KEY_END];
    return std::vector<std::string>(begin, end);
}

SimulateOptions::SimulateOptions(const Dictionary*)
{
    assert(0);
}

void SimulateOptions::setIntegrator(Integrator::IntegratorId value)
{
    // set the value
    integrator = value;

    // adjust the value of the VARIABLE_STEP based on wether we are choosing
    // stochastic or deterministic integrator.
    bool vs = false;

    if (rr::IntegratorFactory::getIntegratorType(value) == Integrator::STOCHASTIC) {
        vs = rr::Config::getBool(rr::Config::SIMULATEOPTIONS_STOCHASTIC_VARIABLE_STEP);
    }

    else if (rr::IntegratorFactory::getIntegratorType(value) == Integrator::DETERMINISTIC) {
        vs = rr::Config::getBool(rr::Config::SIMULATEOPTIONS_DETERMINISTIC_VARIABLE_STEP);
    }

    if (vs) {
        integratorFlags |= Integrator::VARIABLE_STEP;
    } else {
        integratorFlags &= ~Integrator::VARIABLE_STEP;
    }
}



} /* namespace rr */


