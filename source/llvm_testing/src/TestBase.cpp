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

TestBase::TestBase(const std::string& version, int caseNumber) : model(0)
{
    string fileName = getModelFileName(version, caseNumber);

    if(!fileExists(fileName))
    {
        stringstream msg;
        msg<<"File: "<<fileName<<" don't exist";
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
    Log(lDebug5) << "Read SBML content from file:\n " << sbml
                << "\n============ End of SBML " << endl;

    LLVMModelGenerator generator;

    model = (LLVMExecutableModel*)generator.createModel(sbml, 0, 0, false, false);
}

TestBase::~TestBase()
{
    Log(lInfo) << "deleting model, " << __FUNC__;
    delete model;
}

} /* namespace rr */
