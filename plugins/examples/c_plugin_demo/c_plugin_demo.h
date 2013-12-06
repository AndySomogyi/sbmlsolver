#ifndef c_plugin_demoH
#define c_plugin_demoH
#include "rrPluginsAPIExporter.h"
#include "rrCPluginFramework.h"
//-----------------------------------------------------------
#define cc __cdecl

//Plugin Data
RRHandle        gRR;
RRPluginHandle  gPlugin;

char*           text;
const int       textLen = 2048;
//Plugin functions

const char*     RR_PLUGIN_DECLSPEC cc getImplementationLanguage(void);
const char*     RR_PLUGIN_DECLSPEC cc getName(void);
const char*     RR_PLUGIN_DECLSPEC cc getCategory(void);
bool            RR_PLUGIN_DECLSPEC cc setupCPlugin(RRPluginHandle aPlugin);
bool            RR_PLUGIN_DECLSPEC cc execute(void* userData);
bool            RR_PLUGIN_DECLSPEC cc destroyPlugin(RRPluginHandle plugin);

#endif


#if defined(CG_IDE)
#pragma comment(lib, "rrc_api-static.lib")
#pragma comment(lib, "rrplugins_api-static.lib")
#pragma comment(lib, "rrplugins_c_api-static.lib")
#pragma comment(lib, "roadrunner-static.lib")
#endif
