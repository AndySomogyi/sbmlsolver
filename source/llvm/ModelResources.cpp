/*
 * CachedModel.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: andy
 */

#include "ModelResources.h"

#include <rrLogger.h>

using rr::Logger;
using rr::getLogger;

namespace rrllvm
{

ModelResources::ModelResources()
{
    // TODO Auto-generated constructor stub
    Log(Logger::PRIO_NOTICE) << __PRETTY_FUNCTION__;
}

ModelResources::~ModelResources()
{
    Log(Logger::PRIO_NOTICE) << __PRETTY_FUNCTION__;

    if (errStr->size() > 0)
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
