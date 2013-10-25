/*
 * rrModelGeneratorFactory.cpp
 *
 *  Created on: May 20, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "rrModelGeneratorFactory.h"
#include "c/rrCModelGenerator.h"

#if defined(BUILD_LLVM)
#include "llvm/LLVMModelGenerator.h"
#endif

#include "rrLogger.h"
#include <string>
#include <algorithm>

using namespace std;
namespace rr {

ModelGenerator* ModelGeneratorFactory::createModelGenerator(const string& compiler, const string& tempFolder,
            const string& supportCodeFolder)
{
#if defined(BUILD_LLVM)
    string ucomp = compiler;
    std::transform(ucomp.begin(), ucomp.end(),ucomp.begin(), ::toupper);

    if (ucomp == "LLVM")
    {
        Log(Logger::NOTICE) << "Creating LLVM based model generator.";
        return new rrllvm::LLVMModelGenerator();
    }
#endif

    Log(Logger::NOTICE) << "Creating C based model generator using " << compiler << " compiler.";

    // default (for now...), the old C code generating model generator.
    return new CModelGenerator(tempFolder, supportCodeFolder, compiler);
}

} /* namespace rr */
