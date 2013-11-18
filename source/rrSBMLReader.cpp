// The poco files have issues on windows and have to be included before
// any potential 'windows.h' is included. There is a compile problem
// when windows.h is included BEFORE Poco HTTP Factory
#pragma hdrstop
#include <Poco/URI.h>
#include <Poco/URIStreamOpener.h>

#if defined(WITH_POCO_NET)
#include <Poco/Net/HTTPStreamFactory.h>
#endif

#include <Poco/Mutex.h>
#include <Poco/Path.h>
#include <Poco/File.h>

#include "rrOSSpecifics.h"
#include "rrLogger.h"
#include "rrSBMLReader.h"
#include "rrException.h"
#include <iostream>

using Poco::Mutex;

namespace rr {

// asking for trouble statically registering the factory
static Mutex httpFactoryMutex;
static bool httpFactoryRegistered = false;

/**
 * performs some basic checks to see if the string is an sbml string.
 *
 * This just checks the basic format and checks on characters that are
 * not valid file path characters. So if this string matches this function,
 * there should basically be no way that this is a valid path or uri.
 */
static bool is_sbml(const std::string& str)
{
    //<?xml version="1.0" encoding="UTF-8"?> <sbml level="3" version="1" xmlns="http://www.sbml.org/sbml/level3/version1/core">
    size_t pos = str.find("<");
    if (pos != std::string::npos)
    {
        pos = str.find("?", pos + 1);

        if (pos != std::string::npos)
        {
            pos = str.find("xml", pos + 1);

            if (pos != std::string::npos)
            {
                pos = str.find("?", pos + 3);

                if (pos != std::string::npos)
                {
                    pos = str.find(">", pos + 1);

                    if (pos != std::string::npos)
                    {
                        pos = str.find("<", pos + 1);

                        if (pos != std::string::npos)
                        {
                            pos = str.find("sbml", pos + 1);
                            return pos != std::string::npos;
                        }
                    }
                }
            }
        }
    }
    return false;
}


std::string SBMLReader::read(const std::string& str)
{
    if (is_sbml(str))
    {
        return str;
    }

    // at this point, we have to assume that the string is to be interpreted as a
    // path or uri

    // just in case there are multiple threads...
    httpFactoryMutex.lock();

    // not working on windows yet.
#if defined(__APPLE__)
    if (!httpFactoryRegistered)
    {
        Poco::Net::HTTPStreamFactory::registerFactory();
        httpFactoryRegistered = true;
    }
#endif

    httpFactoryMutex.unlock();

    // default opener accepts local file system paths only.
    Poco::URIStreamOpener &opener = Poco::URIStreamOpener::defaultOpener();

    std::istream* stream = opener.open(str);
    if (stream)
    {
        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(*stream), eos);
        delete stream;
        return s;
    }
    else
    {
        string msg = string(__FUNC__) +  ", could not open " + str + " as a file or uri";
        throw Exception(msg);
    }

    return str;
}

} /* namespace rr */




