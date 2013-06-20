#pragma hdrstop
#include <alloc.h>

#include "rrCPluginFramework.h"
#include "./../../Wrappers/C/rrc_core_api.h"
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

bool rrCallConv setupCPlugin(RRHandle aRR)
{
    gRR = aRR;
    return true;
}

bool rrCallConv	execute(void* userData)
{
	char* text2;
	char* text = createTextMemory(2048);
    strcat(text, "C API Version: ");
    strcat(text, getAPIVersion());
	strcat(text, "\nCPP API Version: ");
    text2 = getCPPAPIVersion(gRR);
    if(text2)
    {
		strcat(text, text2);
    }
    else
    {
    	return false;
    }

	*((char**) userData) = text;
    return true;
}

#pragma comment(lib, "rrc_api.lib")
