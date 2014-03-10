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
#if defined(BUILD_LLVM) && !defined(BUILD_LEGACY_C)

    Log(Logger::LOG_INFORMATION) << "Creating LLVM based model generator.";
    return new rrllvm::LLVMModelGenerator();

#endif

#if defined(BUILD_LLVM) && defined(BUILD_LEGACY_C)
    string ucomp = compiler;
    std::transform(ucomp.begin(), ucomp.end(),ucomp.begin(), ::toupper);

    if (ucomp == "LLVM")
    {
        Log(Logger::LOG_INFORMATION) << "Creating LLVM based model generator.";
        return new rrllvm::LLVMModelGenerator();
    }

    Log(Logger::LOG_INFORMATION) << "Creating C based model generator using " << compiler << " compiler.";

    return new CModelGenerator(tempFolder, supportCodeFolder, compiler);
#endif

#if !defined(BUILD_LLVM) && defined(BUILD_LEGACY_C)

    Log(Logger::LOG_INFORMATION) << "Creating C based model generator using " << compiler << " compiler.";

    // default (for now...), the old C code generating model generator.
    return new CModelGenerator(tempFolder, supportCodeFolder, compiler);
#endif

#if !defined(BUILD_LLVM) && !defined(BUILD_LEGACY_C)
#error Must built at least one ModelGenerator backend, either BUILD_LLVM or BUILD_LEGACY_C
#endif


}

} /* namespace rr */
