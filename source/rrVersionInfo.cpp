#include "rrVersionInfo.h"
#include "rrStringUtils.h"
#include "rrRoadRunner.h"
#include "rrCompiler.h"
#include <sbml/common/libsbml-version.h>


// has to be the very last include, has some static functions that
// get compiled into this unit. 
#include "GitInfo.h"


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
        if (std::string(libsbml::getLibSBMLDottedVersion()) != std::string(LIBSBML_DOTTED_VERSION))
            throw std::runtime_error("libSBML version mismatch between linked / compiled library: " + std::string(libsbml::getLibSBMLDottedVersion()) + " for linked vs. " + std::string(LIBSBML_DOTTED_VERSION) + " for compiled version");
        result += std::string("LibSBML Version: ") + std::string(libsbml::getLibSBMLDottedVersion());

        if (options & VERSIONSTR_GIT_BRANCH)
        {
            result += std::string("; ");
        }
    }

    if (options & VERSIONSTR_GIT_BRANCH)
    {
        result += std::string("Git branch: ") + std::string(getGitBranch());

        if (options & VERSIONSTR_GIT_COMMIT)
        {
            result += std::string("; ");
        }
    }


    if (options & VERSIONSTR_GIT_COMMIT)
    {
        result += std::string("Git commit sha: ") + std::string(getGitLastCommit());
    }        

    return result;
}

std::string getCopyrightStr()
{
    return "(C) 2009-2017  Andy Somogyi, Totte Karlsson, Kyle Medley, Kiri Choi, Matthias Konig, Herbert Sauro, Apache License, Version 2.0";
}


}


