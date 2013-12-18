/**
 * @file rrp_cpp_support.h
 * @brief Plugins API CPP Support Functions.
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * 
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
 * redistribute any piece of this software without proper attribution.
 * 
*/

#ifndef rrp_cpp_supportH
#define rrp_cpp_supportH
#include "rrp_api.h"
#include "rrRoadRunnerData.h"
namespace rrp
{
template <class T>
class Parameter;
class Parameters;
class PluginParameter;
class Plugin;
class PluginManager;


/*!
 \brief Cast a handle to RoadRunner PluginManager pointer, throws if it fails
 \param[in] handle  A RRPluginManagerHandle
 \return Pointer to a PluginManager object
 \ingroup cpp_support
*/
PluginManager*                       castToPluginManager(RRPluginManagerHandle handle);

/*!
 \brief Cast a handle to RoadRunner Plugin pointer, throws if it fails
 \param[in] handle  A RRPluginHandle
 \return Pointer to a Plugin object
 \ingroup cpp_support
*/
Plugin*                             castToPlugin(RRPluginHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameters pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameters instance
 \return Pointer to a parameters instance
 \ingroup cpp_support
*/
Parameters*                         castToParameters(RRParametersHandle handle);

/*!
 \brief Cast a handle to RoadRunner PluginParameter pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a baseparameter instance
 \ingroup cpp_support
*/
PluginParameter*                      castToParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<bool> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<int>* instance
 \ingroup cpp_support
*/
Parameter< bool >*                   castToBoolParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<int> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<int>* instance
 \ingroup cpp_support
*/
Parameter< int >*                   castToIntParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<double> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<double>* instance
 \ingroup cpp_support
*/
Parameter<double>*                  castToDoubleParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<string> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<double>* instance
 \ingroup cpp_support
*/
Parameter<string>*                   castToStringParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<Parameters> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<Parameters>* instance
 \ingroup cpp_support
*/
Parameter<Parameters>*              castToParametersParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunnerData pointer, throws if it fails
 \param[in] handle  A handle to a roadrunnerdata instance
 \return Pointer to a RoadRunnerData instance
 \ingroup cpp_support
*/
rr::RoadRunnerData*                 castToRoadRunnerData(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<RoadRunnerData*> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<RoadRunnerData>* instance
 \note To limit the amount of supported types, the underlying type can only be of Pointer to RoadRunner
  data.
 \ingroup cpp_support
*/
Parameter<rr::RoadRunnerData*>*      castToRoadRunnerDataParameter(RRParameterHandle handle);

//Result data
/*!
 \brief Creates a RRData structure from rr::RoadRunnerData object
 \param[in] data  Input RoadRunner data
 \return A handle to a RRData structure. Null if it fails
 \ingroup cpp_support
*/
RRCDataPtr                          createRRCData(const rr::RoadRunnerData& result);

}

#endif
