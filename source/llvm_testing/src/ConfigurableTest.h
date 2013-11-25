/*
 * ConfigurableTest.h
 *
 *  Created on: Sep 25, 2013
 *      Author: andy
 */

#ifndef CONFIGURABLETEST_H_
#define CONFIGURABLETEST_H_


#include "Configurable.h"

namespace rr
{

class ConfigurableTest : Configurable
{
public:
    ConfigurableTest();
    virtual ~ConfigurableTest();


   virtual _xmlNode *createConfigNode();


   virtual void loadConfig(const _xmlDoc* doc);

    bool test();

    static void testRoadRunner(const std::string& sbml, const std::string configFname);
};

} /* namespace rrllvm */
#endif /* CONFIGURABLETEST_H_ */
