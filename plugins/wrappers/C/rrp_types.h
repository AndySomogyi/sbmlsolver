/**
 * @file rrp_types.h
 * @brief Plugins API Types Header
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib/ for more details.
 *
 * Copyright (C) 2012
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

#ifndef rrp_typesH
#define rrp_typesH
#include "rrc_types.h"
#include "rrp_settings.h"
#if defined(__cplusplus)
namespace rrp { extern "C"  { using namespace rrc;
#endif

/*!rrp_pch.h
 \brief Typedef for plugin event function
 \ingroup pluginRoutines
*/
typedef void (event_cc *PluginEvent)(void*, void*);

 /*!@brief Void pointer to a PluginManager instance */
 typedef void* RRPluginManagerHandle;

/*!@brief Void pointer to a Plugin instance */
typedef void* RRPluginHandle;

/*!@brief Void pointer to a Parameter instance */
typedef void* RRParameterHandle;

/*!@brief Void pointer to a RRParameters instance (Container for parameters) */
typedef void* RRParametersHandle;

#if defined( __cplusplus)
} }//Namespace
#endif

#endif
