#pragma hdrstop
#include "rrCPPPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

CPPPlugin::CPPPlugin(const string& name, const string& cat, RoadRunner* aRR, const PluginManager* pm)
:
Plugin(name, cat, aRR, "CPP", pm)
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

