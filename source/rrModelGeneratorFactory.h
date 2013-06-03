/*
 * rrModelGeneratorFactory.h
 *
 *  Created on: May 20, 2013
 *      Author: andy
 */

#ifndef RRMODELGENERATORFACTORY_H_
#define RRMODELGENERATORFACTORY_H_

#include <string>

namespace rr {

using std::string;
class ModelGenerator;

class ModelGeneratorFactory {
public:
    // TODO generalize with hash map string Poco::Any

	/**
	 * Create a model generator.
	 * Eventauly we will have another static method that returns the names of the available
	 * model generators and their descriptions. Currently we only support CModelGenerator, although
	 * this is ignored and this will always return a CModelGenerator.
	 *
	 * The created object is owned by the caller and must be deleted accordingly.
	 *
	 * @param mgid: the textual id of the model generator
	 */
    static ModelGenerator* createModelGenerator(const string& mgid, const string& tempFolder,
    		const string& supportCodeFolder, const string& compiler);
};

} /* namespace rr */
#endif /* RRMODELGENERATORFACTORY_H_ */
