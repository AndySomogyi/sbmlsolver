#pragma hdrstop
#include "TestPlugin.h"
#include "rrRoadRunner.h"
#include <iostream>

//---------------------------------------------------------------------------
namespace TestPlugin
{
using namespace rr;
using namespace rrp;
using namespace std;

TestPlugin::TestPlugin(rr::RoadRunner* aRR, int testParameter)
:
CPPPlugin("TestPlugin", "No Category", aRR),
mTestCapability("A Plugin Capability", "RunMe", "Exposing the RunMe Function"),
mTestParameter("NrOfIterations", 123, "Hint for Nr of iterations")
{
    mTestCapability.setup("TestPlugin", "SetTimeCourseSelectionList", "A function in a plugin");
    mTestCapability.addParameter(&mTestParameter);
    mCapabilities.add(mTestCapability);
}

TestPlugin::~TestPlugin()
{}

bool TestPlugin::execute(void* userData)
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
TestPlugin* rrpCallConv createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
    return new TestPlugin(aRR);
}

const char* rrpCallConv getImplementationLanguage()
{
    return "CPP";
}

}
#if defined(CG_UI)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "roadrunner-static.lib")
        #pragma comment(lib, "rrPluginsAPI-static.lib")
    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrPluginsAPI.lib")
    #endif
#endif

