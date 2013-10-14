/*
 * TestPluginManager.cpp
 *
 *  Created on: Sep 25, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "TestPluginManager.h"
#include "rrRoadRunner.h"
#include "rrPluginManager.h"

namespace rr
{

TestPluginManager::TestPluginManager()
{
    // TODO Auto-generated constructor stub

}

} /* namespace rr */

bool rr::TestPluginManager::test()
{
    PluginManager pm("/Users/andy/local/plugins", false, 0);

    pm.load();

    std::string s = pm.getConfigurationXML();

    return true;
}
