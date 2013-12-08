#include "rrc_api.h"
#include "rrp_api.h"
#include "c_plugin_demo.h"

const char* cc getImplementationLanguage()
{
    return "C";
}

bool cc destroyPlugin(RRPluginHandle plugin)
{
    //Free any data that the plugin generated..
    freeText(text);
}

const char* cc getName()
{
    return "Plugin Demo";
}

const char* cc getCategory()
{
    return "Demos";
}

bool cc setupCPlugin(RRPluginHandle aPlugin)
{
    gPlugin = aPlugin;
    return true;
}

bool cc execute(void* userData)
{
    char* text2;
    text = createTextMemory(textLen);
    gRR = getRRHandleFromPlugin(gPlugin);

    strcpy(text, "CPP API Version: ");
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


