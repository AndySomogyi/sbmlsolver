#ifndef c_plugin_demoH
#define c_plugin_demoH
#include "../source/rrCPlugin.h"
#include "../wrappers/C/rrp_types.h"
#include "../../wrappers/C/rrc_types.h"
//-----------------------------------------------------------

RRHandle gRR;
PLUGIN_DECLSPEC const char*     rrpCallConv getImplementationLanguage(void);
PLUGIN_DECLSPEC const char*     rrpCallConv getName(void);
PLUGIN_DECLSPEC const char*     rrpCallConv getCategory(void);
PLUGIN_DECLSPEC bool            rrpCallConv setupCPlugin(RRHandle aRR);
PLUGIN_DECLSPEC bool            rrpCallConv execute(void* userData);
PLUGIN_DECLSPEC bool            rrpCallConv destroyPlugin(RRPluginHandle plugin);

#endif



