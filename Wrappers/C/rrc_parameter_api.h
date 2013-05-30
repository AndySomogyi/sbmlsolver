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

#ifndef rrc_parameter_apiH
#define rrc_parameter_apiH
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
 \brief Get string list of parameters for a particular capability
 \param[in] rrHandle, capability_name
 \return Returns RRStringList if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC RRStringArrayHandle rrcCallConv getListOfParameters(RRHandle handle, const char* cap_name);

/*!
 \brief Set a parameter
 \param[in] rrHandle, capability_name and parameter name
 \return Returns true if sucessful, false otherwise
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv setParameter(RRParameterHandle handle, const char* value);

/*!
 \brief Get a parameter handle
 \param[in] rrHandle, capability_name and parameter name
 \return Returns RRParameterHandle if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC RRParameterHandle rrcCallConv getParameter(RRHandle handle, const char* cap_name, const char* parName);

/*!
 \brief Get a parameters value as char*
 \param[in] RRParameterHandle
 \return Returns the parameters value if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC char* rrcCallConv getParameterValueAsString(RRParameterHandle handle);

/*!
 \brief Get a parameters value as a void*
 \param[in] RRParameterHandle
 \return Returns the parameters value as a pointer if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC void* rrcCallConv getParameterValueAsPointer(RRParameterHandle handle);

/*!
 \brief Get a parameters name
 \param[in] RRParameterHandle
 \return Returns the parameters name if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC char* rrcCallConv getParameterName(RRParameterHandle handle);

/*!
 \brief Get a parameters hint
 \param[in] RRParameterHandle
 \return Returns the parameters hint if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC char* rrcCallConv getParameterHint(RRParameterHandle handle);


#if defined(__cplusplus)
}	//Extern "C"

}	//rrc namespace
#endif


#endif
