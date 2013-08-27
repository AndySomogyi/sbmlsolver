#ifndef c_plugin_demoH
#define c_plugin_demoH
#include "rrPluginExporter.h"
#include "rrCPluginFramework.h"
//-----------------------------------------------------------

RRHandle gRR;
const char*    PLUGIN_DECLSPEC rrCallConv getImplementationLanguage(void);
const char*    PLUGIN_DECLSPEC rrCallConv getName(void);
const char*    PLUGIN_DECLSPEC rrCallConv getCategory(void);
bool     PLUGIN_DECLSPEC rrCallConv setupCPlugin(RRHandle aRR);
bool     PLUGIN_DECLSPEC rrCallConv execute(void* userData);
bool     PLUGIN_DECLSPEC rrCallConv destroyPlugin(RRPluginHandle plugin);

#endif



