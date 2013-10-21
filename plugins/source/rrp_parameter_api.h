/**
 * @file rrc_parameter_api.h
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

#ifndef rrp_parameter_apiH
#define rrp_parameter_apiH
#include "rrp_exporter.h"
#include "rrc_types.h"
#include "rrp_types.h"
#include "rrplugins_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrc
{
extern "C"
{
#endif

/*!
 \brief Get string list of parameters for a particular capability
 \param[in] handle Handle to a RoadRunner instance
 \param[in] cap_name pointer to a string holding the name of a capability
 \return Returns a handle to a StringArray if sucessful, NULL otherwise
 \ingroup parameters
*/
RRP_DECLSPEC RRStringArrayPtr rrpCallConv getListOfParameters(RRHandle handle, const char* cap_name);

/*!
 \brief Set a parameter
 \param[in] handle to a Parameter instance
 \param[in] value Pointer to string holding the value to assign to the parameter
 \return Returns true if sucessful, false otherwise
 \ingroup parameters
*/
RRP_DECLSPEC bool rrpCallConv setParameter(RRParameterHandle handle, const char* value);

/*!
 \brief Get a parameter handle
 \param[in] handle Handle to a RoadRunner instance
 \param[in] capability_name Name of a capability
 \param[in] parameter_name Name of a parameter
 \return Returns a handle to a parameter if sucessful, NULL otherwise
 \ingroup parameters
*/
RRP_DECLSPEC RRParameterHandle rrpCallConv getParameter(RRHandle handle, const char* capability_name, const char* parameter_name);

/*!
 \brief Get a parameters value as char*
 \param[in] handle to a Parameter instance
 \return Returns the parameters value if sucessful, NULL otherwise
 \ingroup parameters
*/
RRP_DECLSPEC char* rrpCallConv getParameterValueAsString(RRParameterHandle handle);

/*!
 \brief Get a parameters value as a void*
 \param[in] handle to a Parameter instance
 \return Returns the parameters value as a pointer if sucessful, NULL otherwise
 \ingroup parameters
*/
RRP_DECLSPEC void* rrpCallConv getParameterValueAsPointer(RRParameterHandle handle);

/*!
 \brief Get a parameters name
 \param[in] handle to a Parameter instance
 \return Returns the parameters name if sucessful, NULL otherwise
 \ingroup parameters
*/
RRP_DECLSPEC char* rrpCallConv getParameterName(RRParameterHandle handle);

/*!
 \brief Get a parameters hint
 \param[in] handle to a Parameter instance
 \return Returns the parameters hint if sucessful, NULL otherwise
 \ingroup parameters
*/
RRP_DECLSPEC char* rrpCallConv getParameterHint(RRParameterHandle handle);


#if defined(__cplusplus)
}    //Extern "C"

}    //rrc namespace
#endif


#endif
