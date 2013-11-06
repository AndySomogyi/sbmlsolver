#pragma hdrstop
#include "rrCPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

CPlugin::CPlugin(   const string& name, const string& cat, RoadRunner* aRR,
                    PluginCallBackFnc fn1, PluginCallBackFnc fn2, PluginCallBackFnc fn3)
:
Plugin(name, cat, aRR, fn1, fn2, fn3, "C"),
executeFunction(NULL)
{}

CPlugin::~CPlugin()
{}

string CPlugin::getImplementationLanguage()
{
    return "C";
}

void CPlugin::assignExecuteFunction(executeFnc func)
{
    executeFunction = func;
}

bool CPlugin::execute(void* userData, bool runInThread)
{
    if(executeFunction)
    {
        return executeFunction(userData, runInThread);
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


