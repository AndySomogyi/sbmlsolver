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
{
    //Designed to de-allocate any data in the C plugin
    if(destroyFunction)
    {
        destroyFunction();
    }
}

string CPlugin::getImplementationLanguage()
{
    return "C";
}

bool CPlugin::execute(void* userData, bool useThread)
{
    if(executeFunction)
    {
        return executeFunction(userData, useThread);
    }
    return false;
}


}


