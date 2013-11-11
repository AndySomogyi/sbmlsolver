/**
 * @file rrc_plugin_api.h
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib for more details.
 *
 * Copyright (C) 2012-2013
 *   University of Washington, Seattle, WA, USA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * In plain english this means:
 *
 * You CAN freely download and use this software, in whole or in part, for personal,
 * company internal, or commercial purposes;
 *
 * You CAN use the software in packages or distributions that you create.
 *
 * You SHOULD include a copy of the license in any redistribution you may make;
 *
 * You are NOT required include the source of software, or of any modifications you may
 * have made to it, in any redistribution you may assemble that includes it.
 *
 * YOU CANNOT:
 *
 * redistribute any piece of this software without proper attribution;
*/

#ifndef rrp_apiH
#define rrp_apiH
#include "rrp_exporter.h"
#include "rrp_types.h"
#include "rrp_parameter_api.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrp
{
extern "C"
{
#endif

/*!
 \brief Typedef for plugin callback function
 \ingroup pluginRoutines
*/
typedef void (callback_cc *pluginCallBack)(void*);

/**
 * create an instance of a plugin managager attached to the given RoadRunner instance.
 *
 * A PluginManager manages a collection of "plugins", the PluginManager
 * must be attached to a RoadRunner instance.
 */
RRP_DECLSPEC RRPluginManagerHandle rrp_cc createPluginManager(RRHandle rrHandle);

/**
 * create an instance of a plugin managager attached to the given RoadRunner instance.
 *
 * A PluginManager manages a collection of "plugins", the PluginManager
 * must be attached to a RoadRunner instance.
 */
RRP_DECLSPEC RRPluginManagerHandle rrp_cc createPluginManagerEx(const char* pluginDir,
        bool autoLoad, RRHandle rrHandle);

/**
 * free the plugin manager
 */
RRP_DECLSPEC bool rrp_cc freePluginManager(RRPluginManagerHandle handle);

/*!
 \brief load plugins

 \param[in] handle Handle to a PluginManager instance
 \return Returns true if Plugins are loaded, false otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc loadPlugins(RRPluginManagerHandle handle);

/*!
 \brief load a particular plugin

 \param[in] handle Handle to a PluginManager instance
 \param[in] fileName name of the plugin to load. The plugin name is the plugins shared library name, without path and extension.
 \return Returns a handle to a plugin, NULL if unsuccesfull
 \ingroup pluginRoutines
*/
RRP_DECLSPEC RRPluginHandle rrp_cc loadPlugin(RRPluginManagerHandle handle, const char* pluginName);


/*!
 \brief unload plugins

 \param[in] handle Handle to a PluginManager instance
 \return Returns true if Plugins are unloaded succesfully, false otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc unLoadPlugins(RRPluginManagerHandle handle);

/*!
 \brief unload a particular plugin

 \param[in] handle Handle to a PluginManager instance
 \return Returns true if the Plugin are unloaded succesfully, false otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc unLoadPlugin(RRPluginManagerHandle handle, RRPluginHandle plugin);

/*!
 \brief Get Number of loaded plugins

 \param[in] handle Handle to a PluginManager instance
 \return Returns the number of loaded plugins, -1 if a problem is encountered
 \ingroup pluginRoutines
*/
RRP_DECLSPEC int rrp_cc getNumberOfPlugins(RRPluginManagerHandle handle);

/*!
 \brief GetPluginNames
 \param[in] handle Handle to a PluginManager instance
 \return Returns names for loaded plugins, NULL otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC RRStringArrayPtr rrp_cc getPluginNames(RRPluginManagerHandle handle);

/*!
 \brief GetPluginHandle
 \param[in] handle Handle to a PluginManager instance
 \param[in] pluginName Pointer to string holding the name of a plugin
 \return Returns a handle to a plugin, with name as supplied in the paramter pluginName. Returns NULL if plugin is not found
 \ingroup pluginRoutines
*/
RRP_DECLSPEC RRPluginHandle rrp_cc getPlugin(RRPluginManagerHandle handle, const char* pluginName);

/*!
 \brief GetPluginHandle
 \param[in] handle Handle to a PluginManager instance
 \param[in] id integer denoting the ID of a plugin
 \return Returns a handle to a plugin, with id == id. Returns NULL if plugin is NULL
 \ingroup pluginRoutines
*/
RRP_DECLSPEC RRPluginHandle rrp_cc getPluginByID(RRPluginManagerHandle handle, int id);

//// PLUGIN HANDLE functions
/*!
 \brief GetPluginCapabilities
 \param[in] handle Handle to a plugin
 \return Returns available capabilities for a particular plugin, NULL otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC RRStringArrayPtr rrp_cc getPluginCapabilities(RRPluginHandle handle);

/*!
 \brief Get PluginParameters for a specific capability
 \param[in] handle Handle to a plugin
 \param[in] capability Pointer to a string, holding the name of a capability.
 \return Returns available parameters for a particular capability in a plugin, NULL otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC RRStringArrayPtr rrp_cc getPluginParameters(RRPluginHandle handle, const char* capability);

/*!
 \brief Get a parameter handle in a secific capability
 \param[in] handle Handle to a plugin
 \param[in] parameterName Name of paramter
 \param[in] capabilitiesName Name of capability
 \return Returns a pointer to a parameter for a particular plugin. Returns NULL if absent parameter
 \ingroup pluginRoutines
*/
RRP_DECLSPEC RRParameterHandle rrp_cc getPluginParameter(RRPluginHandle handle, const char* parameterName, const char* capabilitiesName);

/*!
 \brief SetPluginParameter
 \param[in] handle Handle to a plugin
 \param[in] parameterName Name of paramter
 \param[in] value Value of parameter, as string
 \return true if succesful, false otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc setPluginParameter(RRPluginHandle handle, const char* parameterName, const char* value);

/*!
 \brief getPluginName
 \param[in] handle Handle to a plugin
 \return Returns the plugins full name, as a string, NULL otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC char* rrp_cc getPluginName(RRPluginHandle handle);

/*!
 \brief GetPluginInfo
 \param[in] handle Handle to a plugin
 \return Returns info, as a string, for the plugin, NULL otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC char* rrp_cc getPluginInfo(RRPluginHandle handle);

/*!
 \brief executePlugin (PluginName)
 \param[in] handle Handle to a plugin
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc executePlugin(RRPluginHandle handle);

/*!
 \brief executePlugin (PluginName)
 \param[in] handle Handle to a plugin
 \param[in] userData void* pointer to user data. Plugin dependent.
 \param[in] inThread bool indicating if the plugin should be executed in a thread.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc executePluginEx(RRPluginHandle handle, void* userData, bool inAThread);

/*!
 \brief getPluginStatus (PluginName)
 \param[in] handle Handle to a plugin
 \return Returns plugin status if available. NULL otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC char* rrp_cc getPluginStatus(RRPluginHandle handle);

/*!
 \brief getPluginResult (PluginName)
 \param[in] handle Handle to a plugin
 \return Returns plugin result if available. NULL otherwise
 \ingroup pluginRoutines
*/
RRP_DECLSPEC char* rrp_cc getPluginResult(RRPluginHandle handle);

/*!
 \brief resetPlugin (PluginName)
 \param[in] handle Handle to a plugin
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc resetPlugin(RRPluginHandle handle);

/*!
 \brief Assign callback function fired when a plugin starts its work
 \param[in] handle Handle to a plugin
 \param[in] cb function pointers to callback routine
 \param[in] userData void* pointer to user data.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc assignPluginStartedCallBack(RRPluginHandle handle, pluginCallBack cb);

/*!
 \brief Assign callback function fired as a plugin progresses
 \param[in] handle Handle to a plugin
 \param[in] cb function pointers to callback routine
 \param[in] userData void* pointer to user data.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc assignPluginProgressCallBack(RRPluginHandle handle, pluginCallBack cb, void* userData);

/*!
 \brief Assign callback function fired when a plugin finishes its work
 \param[in] handle Handle to a plugin
 \param[in] cb function pointers to callback routine
 \param[in] userData void* pointer to user data.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc assignPluginFinishedCallBack(RRPluginHandle handle, pluginCallBack cb, void* userData);

/*!
 \brief Assign callback functions, and user data to a plugin
 \param[in] handle Handle to a plugin
 \param[in] cb1, cb2 and cb3  function pointers to callback routines
 \param[in] userData void* pointer to user data.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc assignCallBacks(RRPluginHandle handle, pluginCallBack cb1, pluginCallBack cb2, pluginCallBack cb3, void* userData);

/*!
 \brief Hand external data to a plugin
 \param[in] handle Handle to a plugin
 \param[in] userData void* pointer to user data. Plugin dependent.
 \return Returns true or false indicating success/failure
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc setPluginInputData(RRPluginHandle handle, void* userData);


/*!
 \brief check if plugin is actively working
 \param[in] handle Handle to a plugin
 \return Returns true or false indicating i the plugin is busy or not
 \ingroup pluginRoutines
*/
RRP_DECLSPEC bool rrp_cc isPluginWorking(RRPluginHandle handle);


/*!
 \brief Set the plugin manager's capabilities
 \param[in] handle Handle to a PluginManager instance
 \param[out] caps An XML string that specifies the simulators capabilities
 \return Returns true if sucessful
 \ingroup simulation
*/
RRP_DECLSPEC bool rrp_cc setPluginManagerConfigurationXML(RRPluginManagerHandle handle, const char* caps);

/*!
 \brief Get the plugin manager's capabilities


 \param[in] handle Handle to a plugin manager instance
 \return Returns null if it fails, otherwise it returns the simulator's capabilities in the form of an XML string
 \ingroup simulation
*/
RRP_DECLSPEC char* rrp_cc getPluginManagerConfigurationXML(RRPluginManagerHandle handle);

#if defined(__cplusplus)
}    //Extern "C"

}    //rrc namespace
#endif


#endif
