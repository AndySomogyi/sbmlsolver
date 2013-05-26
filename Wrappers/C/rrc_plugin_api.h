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

typedef void (rrcCallConv *pluginCallback)(void*);

/*!
 \brief load plugins

 \return Returns true if Plugins are loaded, false otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrcCallConv loadPlugins(RRHandle handle);

/*!
 \brief unload plugins

 \return Returns true if Plugins are unloaded succesfully, false otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrcCallConv unLoadPlugins(RRHandle handle);

/*!
 \brief Get Number of loaded plugins

 \return Returns the number of loaded plugins, -1 if a problem is encountered
 \ingroup pluginRoutines
*/
C_DECL_SPEC int rrcCallConv getNumberOfPlugins(RRHandle handle);

/*!
 \brief GetPluginNames
 \return Returns names for loaded plugins, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC struct RRStringArray* rrcCallConv getPluginNames(RRHandle handle);

/*!
 \brief GetPluginHandle
 \return Returns a handle to a plugin, with namem name. Returns NULL if plugin is not found
 \ingroup pluginRoutines
*/
C_DECL_SPEC RRPluginHandle rrcCallConv getPlugin(RRHandle handle, const char* pluginName);

/*!
 \brief GetPluginCapabilities
 \return Returns available capabilities for a particular plugin, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC struct RRStringArray* rrcCallConv getPluginCapabilities(RRPluginHandle handle);

/*!
 \brief GetPluginParameters
 \return Returns available parameters for a particular plugin, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC struct RRStringArray* rrcCallConv getPluginParameters(RRPluginHandle handle, const char* capability);

/*!
 \brief GetPluginParameter
 \return Returns a pointer to a parameter for a particular plugin. Returns NULL if absent parameter
 \ingroup pluginRoutines
*/
C_DECL_SPEC RRParameterHandle rrcCallConv getPluginParameter(RRPluginHandle handle, const char* parameterName, const char* capabilitiesName);

/*!
 \brief SetPluginParameter
 \return true if succesful, false otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrcCallConv setPluginParameter(RRPluginHandle handle, const char* parameterName, const char* value);

/*!
 \brief getPluginName
 \param[in] RRPluginHandle handle to plugin
 \return Returns the plugins full name, as a string, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC char* rrcCallConv getPluginName(RRPluginHandle handle);

/*!
 \brief GetPluginInfo
 \param[in] RRPluginHandle handle to plugin
 \return Returns info, as a string, for the plugin, NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC char* rrcCallConv getPluginInfo(RRPluginHandle handle);

/*!
 \brief executePlugin (PluginName)
 \param[in] RRPluginHandle handle to plugin
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrcCallConv executePlugin(RRPluginHandle handle);

/*!
 \brief executePlugin (PluginName)
 \param[in] RRPluginHandle handle to plugin
 \param[in] void*  pointer to user data. Plugin dependent.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrcCallConv executePluginEx(RRPluginHandle handle, void* userData);

/*!
 \brief getPluginResult (PluginName)
 \param[in] RRPluginHandle handle to plugin
 \return Returns plugin result if available. NULL otherwise
 \ingroup pluginRoutines
*/
C_DECL_SPEC char* rrcCallConv getPluginResult(RRPluginHandle handle);

/*!
 \brief resetPlugin (PluginName)
 \param[in] RRPluginHandle handle to plugin
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrcCallConv resetPlugin(RRPluginHandle handle);

/*!
 \brief assignCallbacks
 \param[in] RRPluginHandle handle to plugin
 \param[in] void*  pointer to user data. Plugin dependent.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
C_DECL_SPEC bool rrcCallConv assignCallbacks(RRPluginHandle handle, pluginCallback cb1, pluginCallback cb2, void* userData);


C_DECL_SPEC bool rrcCallConv setInputData(RRPluginHandle handle, void* userData);


#if defined(__cplusplus)
}	//Extern "C"

}	//rrc namespace
#endif


#endif
