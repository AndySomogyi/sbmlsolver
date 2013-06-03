/*
 * rrModelGeneratorFactory.cpp
 *
 *  Created on: May 20, 2013
 *      Author: andy
 */

#include "rrModelGeneratorFactory.h"
#include "rrCModelGenerator.h"

namespace rr {

ModelGenerator* ModelGeneratorFactory::createModelGenerator(const string& mgid, const string& tempFolder,
    		const string& supportCodeFolder, const string& compiler)
{
	return new CModelGenerator(tempFolder, supportCodeFolder, compiler);
}

} /* namespace rr */
