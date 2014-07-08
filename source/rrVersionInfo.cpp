#include "rrVersionInfo.h"
#include "rrStringUtils.h"
#include "rrRoadRunner.h"
#include "rrCompiler.h"
#include <sbml/common/libsbml-version.h>


namespace rr {

std::string getVersionStr(unsigned options)
{
    std::string result = "";

    if (options & VERSIONSTR_BASIC)
    {
        result += std::string(RR_VERSION_STR);

        // if other flags, add a ';' as a separator.
        if ((options & VERSIONSTR_COMPILER)
                || (options & VERSIONSTR_DATE)
                || (options & VERSIONSTR_JITCOMPILER))
        {
            result += std::string("; ");
        }
    }

    if (options & VERSIONSTR_COMPILER)
    {
        result += std::string("Compiler: ") + std::string(RR_COMPILER);

        result += ", C++ version: " + rr::toString((long)__cplusplus);

        if ((options & VERSIONSTR_DATE) || (options & VERSIONSTR_JITCOMPILER))
        {
            result += std::string("; ");
        }
    }

    if (options & VERSIONSTR_JITCOMPILER)
    {
        RoadRunner r;
        Compiler* c = r.getCompiler();

        result += "JIT Compiler: ";
        result += c->getCompiler();
        result += "-";
        result += c->getVersion();

        if (options & VERSIONSTR_DATE)
        {
            result += std::string("; ");
        }

    }

    if (options & VERSIONSTR_DATE)
    {
        result += std::string("Date: ") + std::string( __DATE__ ) + ", " + std::string(__TIME__);

        if (options & VERSIONSTR_LIBSBML)
        {
            result += std::string("; ");
        }
    }

    if (options & VERSIONSTR_LIBSBML)
    {
        result += std::string("LibSBML Version: ") + std::string(LIBSBML_DOTTED_VERSION);
    }

    return result;
}

std::string getCopyrightStr()
{
    return "(C) 2009-2014  Andy Somogyi, Totte Karlsson, HM. Sauro, Apache License, Version 2.0";
}


}


