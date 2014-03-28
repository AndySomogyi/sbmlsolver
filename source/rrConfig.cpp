/*
 * Config.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: andy
 */

#include "rrUtils.h"
#include "rrConfig.h"
#include "rrLogger.h"

#if __cplusplus >= 201103L || defined(_MSC_VER)
#include <memory>
#include <unordered_map>
#else
#include <tr1/memory>
#include <tr1/unordered_map>
#endif

#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <fstream>      // std::ofstream
#include <assert.h>

// TODO When we have gcc 4.4 as minimal compiler, drop poco and use C++ standard regex
#include <Poco/RegularExpression.h>
#include <Poco/Path.h>

// somebody will likely call this multithread and then bitch and moan if
// there is an issue, so lock it.
#include <Poco/Mutex.h>

using Poco::Mutex;
using Poco::RegularExpression;
using std::string;

typedef std::tr1::unordered_map<std::string, int> StringIntMap;

/**
 * strip any leading or trailing whitespace
 */
static std::string strip(const std::string& in)
{
    std::string out;
    std::string::const_iterator b = in.begin(), e = in.end();

    // skipping leading spaces
    while (std::isspace(*b)){
        ++b;
    }

    if (b != e){
        // skipping trailing spaces
        while (std::isspace(*(e-1))){
            --e;
        }
    }

    out.assign(b, e);

    return out;
}

/**
 * basically a simplified variant. The Poco DynamicAny is fairly
 * complicated this needs to be set when loaded, not at a later time
 * in order to be effecient.
 */
struct Value {
    std::string strVal;

    enum Type {STRING, DOUBLE, INT, BOOL};

    Type type;

    double doubleVal;
    int intVal;

    Value(int val) {
        type = INT;
        intVal = val;
        doubleVal = (double)val;
        strVal = rr::toString(val);
    }

    Value(double val) {
        type = DOUBLE;
        doubleVal = val;
        intVal = (int)val;
        strVal = rr::toString(val, "%e");
    }

    Value(bool val) {
        type = BOOL;
        doubleVal = (double)val;
        intVal = (int)val;
        strVal = rr::toString(val);
    }

    Value(const std::string& str) {

        strVal = strip(str);
        type = STRING;

        const char* input = strVal.c_str();
        char* end;

        // check for double
        doubleVal = strtod(input, &end);
        if (*input != '\0' && end != input && *end == '\0')
        {
            intVal = (int)doubleVal;
            type = DOUBLE;
            return;
        }

        // check for int
        intVal = strtol(input, &end, 10);
        if (*input != '\0' && end != input && *end == '\0')
        {
            doubleVal = (double)intVal;
            type = INT;
            return;
        }

        // check for bool
        std::string bstr = strVal;
        std::transform(bstr.begin(), bstr.end(),bstr.begin(), ::toupper);

        if (bstr == "TRUE") {
            strVal = "TRUE";
            intVal = 1;
            doubleVal = 1;
            type = BOOL;
            return;
        }

        if (bstr == "FALSE") {
            strVal = "FALSE";
            intVal = 0;
            doubleVal = 0;
            type = BOOL;
            return;
        }

        // its a string, zero out the numeric types
        intVal = 0;
        doubleVal = 0;
    }
};




static Value values[] =  {
    Value(false),    // LOADSBMLOPTIONS_CONSERVED_MOIETIES
    Value(false),    // LOADSBMLOPTIONS_RECOMPILE
    Value(false),    // LOADSBMLOPTIONS_READ_ONLY
    Value(true),     // LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS
    Value(false),    // LOADSBMLOPTIONS_OPTIMIZE_GVN
    Value(false),    // LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION
    Value(false),    // LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING
    Value(false),    // LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION
    Value(false),    // LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION
    Value(false),    // LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER
    Value(false),    // LOADSBMLOPTIONS_USE_MCJIT
    Value(50),       // SIMULATEOPTIONS_STEPS,
    Value(5),        // SIMULATEOPTIONS_DURATION,
    Value(1.e-10),   // SIMULATEOPTIONS_ABSOLUTE,
    Value(1.e-5),    // SIMULATEOPTIONS_RELATIVE,
    Value(true),     // SIMULATEOPTIONS_STRUCTURED_RESULT,
    Value(false),    // SIMULATEOPTIONS_STIFF,
    Value(false),    // SIMULATEOPTIONS_MULTI_STEP,
    Value(-1),       // SIMULATEOPTIONS_INITIAL_TIMESTEP,
    Value(-1),       // SIMULATEOPTIONS_MINIMUM_TIMESTEP,
    Value(-1),       // SIMULATEOPTIONS_MAXIMUM_TIMESTEP,
    Value(-1),       // SIMULATEOPTIONS_MAXIMUM_NUM_STEPS
    Value(false)     // ROADRUNNER_DISABLE_WARNINGS
};

static bool initialized = false;
static Mutex configMutex;

static void readDefaultConfig() {
    Mutex::ScopedLock lock(configMutex);

    if(!initialized) {
        assert(rr::Config::CONFIG_END == sizeof(values) / sizeof(Value) &&
                "values array size different than CONFIG_END");

        string confPath = rr::Config::getConfigFilePath();

        try {
            if(confPath.size() > 0) {
                rr::Config::readConfigFile(confPath);
            }
        }
        catch(std::exception& e) {
            Log(rr::Logger::LOG_WARNING) << "error reading configuration file: "
                    << confPath << ", " << e.what();
        }
        initialized = true;
    }
}

/**
 * load the names of the keys and values into a map
 */
static void getKeyNames(StringIntMap& keys)
{
    keys["LOADSBMLOPTIONS_CONSERVED_MOIETIES"] = rr::Config::LOADSBMLOPTIONS_CONSERVED_MOIETIES;
    keys["LOADSBMLOPTIONS_RECOMPILE"] = rr::Config::LOADSBMLOPTIONS_RECOMPILE;
    keys["LOADSBMLOPTIONS_READ_ONLY"] = rr::Config::LOADSBMLOPTIONS_READ_ONLY;
    keys["LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS"] = rr::Config::LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS;
    keys["LOADSBMLOPTIONS_OPTIMIZE_GVN"] = rr::Config::LOADSBMLOPTIONS_OPTIMIZE_GVN;
    keys["LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION"] = rr::Config::LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION;
    keys["LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING"] = rr::Config::LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING;
    keys["LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION"] = rr::Config::LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION;
    keys["LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION"] = rr::Config::LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION;
    keys["LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER"] = rr::Config::LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER;
    keys["LOADSBMLOPTIONS_USE_MCJIT"] = rr::Config::LOADSBMLOPTIONS_USE_MCJIT;
    keys["SIMULATEOPTIONS_STEPS"] = rr::Config::SIMULATEOPTIONS_STEPS;
    keys["SIMULATEOPTIONS_DURATION"] = rr::Config::SIMULATEOPTIONS_DURATION;
    keys["SIMULATEOPTIONS_ABSOLUTE"] = rr::Config::SIMULATEOPTIONS_ABSOLUTE;
    keys["SIMULATEOPTIONS_RELATIVE"] = rr::Config::SIMULATEOPTIONS_RELATIVE;
    keys["SIMULATEOPTIONS_STRUCTURED_RESULT"] = rr::Config::SIMULATEOPTIONS_STRUCTURED_RESULT;
    keys["SIMULATEOPTIONS_STIFF"] = rr::Config::SIMULATEOPTIONS_STIFF;
    keys["SIMULATEOPTIONS_MULTI_STEP"] = rr::Config::SIMULATEOPTIONS_MULTI_STEP;
    keys["SIMULATEOPTIONS_INITIAL_TIMESTEP"] = rr::Config::SIMULATEOPTIONS_INITIAL_TIMESTEP;
    keys["SIMULATEOPTIONS_MINIMUM_TIMESTEP"] = rr::Config::SIMULATEOPTIONS_MINIMUM_TIMESTEP;
    keys["SIMULATEOPTIONS_MAXIMUM_TIMESTEP"] = rr::Config::SIMULATEOPTIONS_MAXIMUM_TIMESTEP;
    keys["SIMULATEOPTIONS_MAXIMUM_NUM_STEPS"] = rr::Config::SIMULATEOPTIONS_MAXIMUM_NUM_STEPS;
    keys["ROADRUNNER_DISABLE_WARNINGS"] = rr::Config::ROADRUNNER_DISABLE_WARNINGS;


    assert(rr::Config::CONFIG_END == sizeof(values) / sizeof(Value) &&
            "values array size different than CONFIG_END");
    assert(rr::Config::CONFIG_END == keys.size()  &&
            "number of keys in map does not match static values");
}


namespace rr
{

std::string Config::getString(Keys key)
{
    readDefaultConfig();
    return values[key].strVal;
}

int Config::getInt(Keys key)
{
    readDefaultConfig();
    return values[key].intVal;
}

double Config::getDouble(Keys key)
{
    readDefaultConfig();
    return values[key].doubleVal;
}

std::string Config::getConfigFilePath()
{
    // check env var
    const char* env = std::getenv("ROADRUNNER_CONFIG");
    std::string path;
    Poco::Path ppath;

    Poco::Path::home();

    Log(rr::Logger::LOG_DEBUG) << "trying config file from ROADRUNNER_CONFIG "
        << (env ? env : "NULL");

    if (env && rr::fileExists(env, 4))
    {
        return env;
    }

    // check home dir
    ppath.assign(Poco::Path::home());
    ppath.setFileName("roadrunner.conf");
    path = ppath.toString();
    Log(rr::Logger::LOG_DEBUG) << "trying config file " << path;
    if (rr::fileExists(path, 4))
    {
        return path;
    }

    ppath.setFileName(".roadrunner.conf");
    path = ppath.toString();
    Log(rr::Logger::LOG_DEBUG) << "trying config file " << path;
    if (rr::fileExists(path, 4))
    {
        return path;
    }


    // check in library dir
    ppath.assign(rr::getCurrentSharedLibDir());
    ppath.setFileName("roadrunner.conf");
    path = ppath.toString();
    Log(rr::Logger::LOG_DEBUG) << "trying config file " << path;
    if (rr::fileExists(path, 4))
    {
        return path;
    }

    // check one level up
    ppath.assign(rr::getCurrentSharedLibDir());
    ppath.popDirectory();
    ppath.setFileName("roadrunner.conf");
    path = ppath.toString();
    Log(rr::Logger::LOG_DEBUG) << "trying config file " << path;
    if (rr::fileExists(path, 4))
    {
        return path;
    }

    return "";
}

void Config::setValue(Keys key, const std::string& str)
{
    readDefaultConfig();
    values[key] = Value(str);
}

void Config::setValue(Keys key, int i)
{
    readDefaultConfig();
    values[key] = Value(i);
}

void Config::setValue(Keys key, double d)
{
    readDefaultConfig();
    values[key] = Value(d);
}

void Config::setValue(Keys key, bool b)
{
    readDefaultConfig();
    values[key] = Value(b);
}

void Config::readConfigFile(const std::string& path)
{
    Mutex::ScopedLock lock(configMutex);

    const Poco::RegularExpression re("^\\s*(\\w*)\\s*:\\s*(.*)\\s*$", RegularExpression::RE_CASELESS);
    StringIntMap keys;
    std::ifstream in(path.c_str());

    if(!in) {
        throw std::ifstream::failure("could not open " + path + " for reading");
    }

    getKeyNames(keys);

    std::string line;

    while(std::getline(in, line)) {
        std::vector<std::string> matches;

        int nmatch = re.split(line, matches);

        if (nmatch == 3)
        {
            StringIntMap::const_iterator i = keys.find(matches[1]);
            if(i != keys.end()) {
                values[i->second] = Value(matches[2]);
                Log(Logger::LOG_INFORMATION) << "read key " << i->first << " with value: " << values[i->second].strVal;
            } else {
                Log(Logger::LOG_WARNING) << "invalid key: \"" << matches[1] << "\" in " << path;
            }
        }
    }

    initialized = true;
}

void Config::writeConfigFile(const std::string& path)
{
    Mutex::ScopedLock lock(configMutex);

    std::ofstream out(path.c_str());

    if(!out) {
        throw std::ofstream::failure("could not open " + path + " for writing");
    }

    StringIntMap keys;
    std::ifstream in(path.c_str());

    getKeyNames(keys);

    for (StringIntMap::const_iterator i = keys.begin(); i != keys.end(); ++i) {
        out << i->first << ": " << values[i->second].strVal << std::endl;
    }
}

}
