/*
 * rrLLVMModelGenerator.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#include "rrLLVMModelGenerator.h"

namespace rr
{

LLVMModelGenerator::LLVMModelGenerator()
{
    // TODO Auto-generated constructor stub

}

LLVMModelGenerator::~LLVMModelGenerator()
{
    // TODO Auto-generated destructor stub
}

bool LLVMModelGenerator::setTemporaryDirectory(const string& path)
{
    return false;
}

string LLVMModelGenerator::getTemporaryDirectory()
{
    return "";
}

ExecutableModel* LLVMModelGenerator::createModel(const string& sbml,
        LibStructural* ls, NOMSupport* nom, bool forceReCompile,
        bool computeAndAssignConsevationLaws)
{
    return 0;
}

Compiler* LLVMModelGenerator::getCompiler()
{
    return 0;
}

bool LLVMModelGenerator::setCompiler(const string& compiler)
{
    return false;
}

} /* namespace rr */
