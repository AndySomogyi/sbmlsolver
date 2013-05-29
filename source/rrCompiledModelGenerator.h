/*
 * rrCompiledModelGenerator.h
 *
 *  Created on: May 26, 2013
 *      Author: andy
 */

#ifndef RRCOMPILEDMODELGENERATOR_H_
#define RRCOMPILEDMODELGENERATOR_H_

#include "rrModelGenerator.h"
#include "rrModelSharedLibrary.h"
#include "rrCodeBuilder.h"
#include "rrCompiler.h"

namespace rr {

/**
 * Both compiled model generators (C and CSharp) share a lot of functionality,
 * so implement that here.
 */
class RR_DECLSPEC CompiledModelGenerator : public ModelGenerator
{
	public:
    								CompiledModelGenerator();
    	virtual 				   ~CompiledModelGenerator();
};

} /* namespace rr */
#endif /* RRCOMPILEDMODELGENERATOR_H_ */
