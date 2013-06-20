#ifndef c_plugin_demoH
#define c_plugin_demoH
#include <stdbool.h>
#include "../../Wrappers/C/rrc_types.h"
#include "rrPluginExporter.h"
#include "rrOSSpecifics.h"
#include "rrcPluginFramework.h"
//---------------------------------------------------------------------------

//RRPluginData*			gPluginData;
RRHandle				gRR;
char* 					PLUGIN_DECLSPEC rrCallConv 	getImplementationLanguage(void);
char*                   PLUGIN_DECLSPEC rrCallConv 	getName(void);
char*                   PLUGIN_DECLSPEC rrCallConv 	getCategory(void);
bool					PLUGIN_DECLSPEC rrCallConv	setupCPlugin(RRHandle aRR);
bool					PLUGIN_DECLSPEC rrCallConv	execute(void* userData);
bool					PLUGIN_DECLSPEC	rrCallConv	destroyPlugin(RRPluginHandle plugin);

#endif



