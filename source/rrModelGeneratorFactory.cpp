/*
 * rrModelGeneratorFactory.cpp
 *
 *  Created on: May 20, 2013
 *      Author: andy
 */

#include "rrModelGeneratorFactory.h"
#include "rrCModelGenerator.h"
#include "rrLLVMModelGenerator.h"
#include "rrLogger.h"
#include <algorithm>
#include <string>


namespace rr {

ModelGenerator* ModelGeneratorFactory::createModelGenerator(const string& mgid, const string& tempFolder,
            const string& supportCodeFolder, const string& compiler)
{
#if defined(BUILD_LLVM)
    string ucomp = compiler;
    std::transform(ucomp.begin(), ucomp.end(),ucomp.begin(), ::toupper);

    if (ucomp == "LLVM")
    {
        Log(Logger::PRIO_NOTICE) << "Creating LLVM based model generator";
        return new LLVMModelGenerator();
    }
#endif

    // default (for now...), the old C code generating model generator.
    return new CModelGenerator(tempFolder, supportCodeFolder, compiler);
}

} /* namespace rr */
