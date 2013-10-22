#pragma hdrstop
#include "rrCPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

CPlugin::CPlugin(const string& name, const string& cat, RoadRunner* aRR,PluginWorkStartedCB fn1, PluginWorkFinishedCB fn2)
:
Plugin(name, cat, aRR, fn1, fn2, "C"),
executeFunction(NULL)
{}

CPlugin::~CPlugin()
{}

string CPlugin::getImplementationLanguage()
{
    return "C";
}

void CPlugin::assignExecuteFunction(executeFunc func)
{
    executeFunction = func;
}

bool CPlugin::execute(void* userData)
{
    if(executeFunction)
    {
        return executeFunction(userData);
    }
    return false;
}

_xmlNode* CPlugin::createConfigNode()
{
    return 0;
}

void CPlugin::loadConfig(const _xmlDoc* doc)
{
}

}


