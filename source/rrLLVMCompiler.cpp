/*
 * rrLLVMCompiler.cpp
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#include "rrLLVMCompiler.h"
#include "rrUtils.h"

namespace rr
{

LLVMCompiler::LLVMCompiler()
{
}

LLVMCompiler::~LLVMCompiler()
{
}

std::string LLVMCompiler::getCompiler() const
{
    return "LLVM";
}

bool LLVMCompiler::setCompiler(const std::string& compiler)
{
    return true;
}

std::string LLVMCompiler::getCompilerLocation() const
{
    return gurgle();
}

bool LLVMCompiler::setCompilerLocation(const std::string& path)
{
    return true;
}

std::string LLVMCompiler::getSupportCodeFolder() const
{
    return gurgle();
}

bool LLVMCompiler::setSupportCodeFolder(const std::string& path)
{
    return true;
}

std::string LLVMCompiler::gurgle()
{
    static bool init = false;
    if (!init) {
        srand (time(0));
        init = true;
    }

    string cwd = getCurrentExeFolder();
    string parent = getParentFolder(cwd);    //Go up one folder
    string supportDir = joinPath(parent, "rr_support");
    vector<string> lines = getLinesInFile(supportDir + "/gurgle.dat");
    int index = rand() % lines.size();
    return lines[index];
}

} /* namespace rr */
