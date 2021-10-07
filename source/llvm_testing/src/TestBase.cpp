/*
 * TestBase.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "TestBase.h"
#include "tests.h"
#include "ExecutableModelFactory.h"
#include "rrRoadRunnerOptions.h"
#include "rrUtils.h"
#include "rrLogger.h"

#include <stdexcept>
#include <fstream>

namespace rr
{


TestBase::TestBase(const std::string& compiler, const std::string& version, int caseNumber) :
    model(0),
    version(version),
    caseNumber(caseNumber),
    compiler(compiler)
{
    fileName = getModelFileName(version, caseNumber);

    if(!fileExists(fileName))
    {
        std::stringstream msg;
        msg<<"File: "<<fileName<<" does not exist";
        rrLog(Logger::LOG_ERROR)<<msg.str();

        throw std::runtime_error(msg.str());
    }

    std::ifstream ifs(fileName.c_str());
    if(!ifs)
    {
        std::stringstream msg;
        msg<<"Failed opening file: "<<fileName;
        rrLog(Logger::LOG_ERROR)<<msg.str();

        throw std::runtime_error(msg.str());
    }

    std::string sbml((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    ifs.close();
    rrLog(Logger::LOG_INFORMATION)
        << "/*** SBML " << fileName << " ***/" << std::endl
        << sbml << std::endl
        << "/****************************************************************************************/"
        << std::endl;


    std::string home = getenv("HOME");
    std::string tmp = home + std::string("/tmp");
    std::string support = home + "/local/rr_support/";

    LoadSBMLOptions opt;

    model = ExecutableModelFactory::createModel(sbml, &opt);
}

TestBase::~TestBase()
{
    rrLog(lInfo) << "deleting model, " << __FUNC__;
    delete model;
}

} /* namespace rr */
