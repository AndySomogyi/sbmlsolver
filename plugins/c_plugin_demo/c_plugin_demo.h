#ifndef c_plugin_demoH
#define c_plugin_demoH
//#include "rrPluginExporter.h"
#include "../source/rrCPlugin.h"
//-----------------------------------------------------------

RRHandle gRR;
PLUGIN_DECLSPEC const char*     rrpCallConv getImplementationLanguage(void);
PLUGIN_DECLSPEC const char*     rrpCallConv getName(void);
PLUGIN_DECLSPEC const char*     rrpCallConv getCategory(void);
PLUGIN_DECLSPEC bool            rrpCallConv setupCPlugin(RRHandle aRR);
PLUGIN_DECLSPEC bool            rrpCallConv execute(void* userData);
PLUGIN_DECLSPEC bool            rrpCallConv destroyPlugin(RRPluginHandle plugin);

#endif



