#ifndef rrc_plugin_apiH
#define rrc_plugin_apiH
#include "rrc_exporter.h"
#include "rrc_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrc
{
extern "C"
{
#endif

/*!
 \brief load plugins

 \return Returns true if Plugins are loaded, false otherwise
 \ingroup pluginRoutines
*/

C_DECL_SPEC bool rrCallConv loadPlugins(RRHandle handle);

/*!
 \brief unload plugins

 \return Returns true if Plugins are unloaded succesfully, false otherwise
 \ingroup pluginRoutines
*/

C_DECL_SPEC bool rrCallConv unLoadPlugins(RRHandle handle);

/*!
 \brief Get Number of loaded plugins

 \return Returns the number of loaded plugins, -1 if a problem is encountered
 \ingroup pluginRoutines
*/

C_DECL_SPEC int rrCallConv getNumberOfPlugins(RRHandle handle);

/*!
 \brief GetPluginNames
 \return Returns names for loaded plugins, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC struct RRStringArray* rrCallConv getPluginNames(RRHandle handle);

/*!
 \brief GetPluginCapabilities
 \return Returns available capabilities for a particular plugin, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC struct RRStringArray* rrCallConv getPluginCapabilities(RRHandle handle, const char* pluginName);

/*!
 \brief GetPluginParameters
 \return Returns available parameters for a particular plugin, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC struct RRStringArray* rrCallConv getPluginParameters(RRHandle handle, const char* pluginName, const char* capability);

/*!
 \brief GetPluginParameter
 \return Returns a pointer to a parameter for a particular plugin. Returns NULL if absent parameter
 \ingroup pluginRoutines
*/
C_DECL_SPEC RRParameterHandle rrCallConv getPluginParameter(RRHandle handle, const char* pluginName, const char* parameterName, const char* capabilitiesName = NULL);

/*!
 \brief SetPluginParameter
 \return true if succesful, false otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrCallConv setPluginParameter(RRHandle handle, const char* pluginName, const char* parameterName, const char* value);

/*!
 \brief GetPluginInfo (PluginName)
 \param[in] string name of queried plugin
 \return Returns info, as a string, for the plugin, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC char* rrCallConv getPluginInfo(RRHandle handle, const char* name);

/*!
 \brief executePlugin (PluginName)
 \param[in] string name of plugin to execute
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/

C_DECL_SPEC bool rrCallConv executePlugin(RRHandle handle, const char* name);


#if defined(__cplusplus)
}	//Extern "C"
}	//namespace
#endif

#endif
