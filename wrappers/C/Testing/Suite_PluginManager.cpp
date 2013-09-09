/*
 * Suite_PluginManager.cpp
 *
 *  Created on: Sep 9, 2013
 *      Author: andy
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "UnitTest++.h"
#include "rrc_api.h"
#include "rrc_plugin_api.h"
#include "rrUtils.h"
#include "rrIniFile.h"
#include "rrException.h"

using namespace UnitTest;
using namespace std;
using namespace rr;
using namespace rrc;

extern string gTempFolder;
extern string gCompiler;

SUITE(PLUGINMANAGER_TESTS)
{

TEST(NOM_TEST_DATA_FILES)
{
    string sec("PLUGINMANAGER_TESTS");
    string key("test");

    RRHandle rr = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());

    RRPluginManagerHandle pm = createRRPluginManager(rr);

    getNumberOfPlugins(pm);


   freeRRPluginManager(pm);

   freeRRInstance(rr);



}

}

