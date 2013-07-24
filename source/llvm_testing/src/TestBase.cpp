/*
 * TestBase.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */

#include "TestBase.h"
#include "tests.h"

#include "rrLogger.h"

#include <stdexcept>

namespace rr
{
using namespace std;

TestBase::TestBase(const std::string& version, int caseNumber) : 
    model(0),
    version(version),
    caseNumber(caseNumber)
{
    fileName = getModelFileName(version, caseNumber);

    if(!fileExists(fileName))
    {
        stringstream msg;
        msg<<"File: "<<fileName<<" does not exist";
        Log(lError)<<msg.str();

        throw std::runtime_error(msg.str());
    }

    ifstream ifs(fileName.c_str());
    if(!ifs)
    {
        stringstream msg;
        msg<<"Failed opening file: "<<fileName;
        Log(lError)<<msg.str();

        throw std::runtime_error(msg.str());
    }

    std::string sbml((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    ifs.close();
    Log(Logger::PRIO_INFORMATION) 
        << "/*** SBML " << fileName << " ***/" << endl
        << sbml << endl
        << "/****************************************************************************************/" 
        << endl;

    LLVMModelGenerator generator;

    model = (LLVMExecutableModel*)generator.createModel(sbml, 0, 0, false, false);
}

TestBase::~TestBase()
{
    Log(lInfo) << "deleting model, " << __FUNC__;
    delete model;
}

} /* namespace rr */
