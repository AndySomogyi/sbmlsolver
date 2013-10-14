/*
 * CachedModel.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "ModelResources.h"

#include <rrLogger.h>

using rr::Logger;
using rr::getLogger;

namespace rrllvm
{

ModelResources::ModelResources() :
        symbols(0), executionEngine(0), context(0), errStr(0)
{
    // the reset of the ivars are assigned by the generator,
    // and in an exception they are not, does not matter as
    // we don't have to delete them.
}

ModelResources::~ModelResources()
{
    Log(Logger::PRIO_DEBUG) << __FUNC__;

    if (errStr && errStr->size() > 0)
    {
        Log(Logger::PRIO_WARNING) << "Non-empty LLVM ExecutionEngine error string: " << *errStr;
    }

    delete symbols;
    // the exe engine owns all the functions
    delete executionEngine;
    delete context;
    delete errStr;
}

} /* namespace rrllvm */
