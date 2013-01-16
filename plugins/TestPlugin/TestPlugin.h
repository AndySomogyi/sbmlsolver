#ifndef TestPluginH
#define TestPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

class TestPlugin : public rr::Plugin
{
	private:

    public:
    					TestPlugin();
					   ~TestPlugin();
		bool			Execute();

};

/////////////////////////////////////////////////////////////////////////
// Plugin factory function
extern "C"
{
__declspec(dllexport) rr::Plugin* Create_Plugin();

// Plugin cleanup function
__declspec(dllexport) bool Release_Plugin (rr::Plugin *plugin);

}

#endif
