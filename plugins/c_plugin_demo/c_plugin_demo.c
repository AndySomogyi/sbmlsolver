#include "rrc_api.h"
#include "c_plugin_demo.h"

char* rrCallConv getImplementationLanguage()
{
	return "C";
}

char* rrCallConv getName()
{
	return "Plugin Demo";
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
    strcat(text, "CPP API Version: ");
    text2 = getCPPAPIVersion(gRR);
    if(text2)
    {
		strcat(text, text2);
        freeText(text2);
    }
    else
    {
    	return false;
    }

	*((char**) userData) = text;
    return true;
}

#if defined(CG_IDE)
#pragma comment(lib, "rrc_api.lib")
#endif
