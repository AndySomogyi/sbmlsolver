#pragma hdrstop
#include "TestPlugin.h"
#include "rrCapability.h"
#include "rrRoadRunner.h"
#include "rrParameter.h"

#if defined(CG_UI)
    #if defined(STATIC_BUILD)
    	#pragma comment(lib, "roadrunner-static.lib")
    #else
    	#pragma comment(lib, "roadrunner.lib")
    #endif
#endif
//---------------------------------------------------------------------------
namespace TestPlugin
{
using namespace rr;

TestPlugin::TestPlugin(rr::RoadRunner* aRR, int testParameter)
:
Plugin("TestPlugin", "No Category", aRR),
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
    	mRR->setTimeCourseSelectionList("S2, S1");
    }
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new TestPlugin(aRR);
}

}

