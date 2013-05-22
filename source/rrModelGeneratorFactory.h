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
    // TODO generalize with hash map string Poco::Any
    static ModelGenerator* createModelGenerator(const string& tempFolder, const string& supportCodeFolder, const string& compiler);
};

} /* namespace rr */
#endif /* RRMODELGENERATORFACTORY_H_ */
