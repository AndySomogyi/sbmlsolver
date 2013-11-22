#pragma hdrstop
#include "rrCPPPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

CPPPlugin::CPPPlugin(const string& name, const string& cat, RoadRunner* aRR, const PluginManager* pm,
                        PluginCallBackFnc fn1, PluginCallBackFnc fn2, PluginCallBackFnc fn3)
:
Plugin(name, cat, aRR, fn1, fn2, fn3, "CPP", pm)
{}

CPPPlugin::~CPPPlugin()
{}

_xmlNode* CPPPlugin::createConfigNode(){return NULL;}
void CPPPlugin::loadConfig(const _xmlDoc* doc){}

string CPPPlugin::getImplementationLanguage()
{
    return "CPP";
}

}

