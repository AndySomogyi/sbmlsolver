#pragma hdrstop
#include "rrCPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

CPlugin::CPlugin(   const string& name, const string& cat, RoadRunner* aRR)
:
Plugin(name, cat, aRR, "C"),
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

bool CPlugin::execute(void* userData, bool useThread)
{
    if(executeFunction)
    {
        return executeFunction(userData, useThread);
    }
    return false;
}

//_xmlNode* CPlugin::createConfigNode()
//{
//    return 0;
//}
//
//void CPlugin::loadConfig(const _xmlDoc* doc)
//{
//}

}


