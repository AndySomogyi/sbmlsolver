#pragma hdrstop
#include <alloc.h>

#include "rrCPluginFramework.h"
#include "./../../Wrappers/C/rrc_api.h"
#include "./../../Wrappers/C/rrc_utilities.h"
#include "c_plugin_demo.h"

char* rrCallConv getImplementationLanguage()
{
	return "C";
}

char* rrCallConv getName()
{
	return "c_plugin_demo";
}

char* rrCallConv getCategory()
{
	return "Demos";
}

RRPluginDataHandle  rrCallConv	createCPluginData(RRHandle aRR)
{
    //allocate a new object and return it
    gPluginData = (RRPluginDataHandle) calloc(1, sizeof(struct RRPluginData));
	gPluginData->mRR = aRR;
    return gPluginData;
}

bool rrCallConv	execute(void* userData)
{
	*((char**) userData) = getAPIVersion();
    return true;
}

#pragma comment(lib, "rrc_api.lib")
