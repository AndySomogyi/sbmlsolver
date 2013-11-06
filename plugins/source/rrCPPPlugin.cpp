#pragma hdrstop
#include "rrCPPPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

CPPPlugin::CPPPlugin(const string& name, const string& cat, RoadRunner* aRR,
                        PluginCallBackFnc fn1, PluginCallBackFnc fn2, PluginCallBackFnc fn3)
:
Plugin(name, cat, aRR, fn1, fn2, fn3, "CPP")
{}

CPPPlugin::~CPPPlugin()
{}


string CPPPlugin::getImplementationLanguage()
{
    return "CPP";
}

}

