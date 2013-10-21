#pragma hdrstop
#include "rrCPPPlugin.h"
//---------------------------------------------------------------------------

namespace rr
{

CPPPlugin::CPPPlugin(const string& name, const string& cat, RoadRunner* aRR,PluginWorkStartedCB fn1, PluginWorkFinishedCB fn2)
:
Plugin(name, cat, aRR, fn1, fn2, "CPP")
{}

CPPPlugin::~CPPPlugin()
{}


string CPPPlugin::getImplementationLanguage()
{
    return "C";
}

}

