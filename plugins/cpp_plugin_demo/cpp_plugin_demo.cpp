#pragma hdrstop
#include "cpp_plugin_demo.h"
#include "rrRoadRunner.h"
#include <iostream>

//---------------------------------------------------------------------------
namespace cpp_plugin_demo
{
using namespace rr;
using namespace rrp;
using namespace std;

TestPlugin::TestPlugin(RoadRunner* aRR, int testParameter)
:
CPPPlugin("TestPlugin", "No Category", aRR),
mTestCapability("A Plugin Capability", "Exposing the RunMe Function"),
mTestParameter("NrOfIterations", 123, "Hint for Nr of iterations")
{
    mTestCapability.setup("TestPlugin", "A function in a plugin");
    mTestCapability.addParameter(&mTestParameter);
    mCapabilities.add(mTestCapability);
}

TestPlugin::~TestPlugin()
{}

bool TestPlugin::execute(void* userData, bool inThread)
{
    cout<<"Executing plugin...\n";
    if(mRR)
    {
        StringList aList("S2, S1",", ");
        mRR->setSelections(aList);
    }
    return true;
}

// Plugin raw C functions
TestPlugin* plugins_cc createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
    return new TestPlugin(aRR);
}

const char* plugins_cc getImplementationLanguage()
{
    return "CPP";
}

}

