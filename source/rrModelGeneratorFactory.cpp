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

namespace rr {

ModelGenerator* ModelGeneratorFactory::createModelGenerator(const string& mgid, const string& tempFolder,
            const string& supportCodeFolder, const string& compiler)
{
#if defined(BUILD_LLVM)
    if (compiler == "LLVM")
    {
        return new LLVMModelGenerator();
    }
#endif

    // default (for now...), the old C code generating model generator.
    return new CModelGenerator(tempFolder, supportCodeFolder, compiler);
}

} /* namespace rr */
