/**
 * @file rrp_parameter_api.h
 * @brief Plugins API Parameters Header
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
#include "rrp_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrp { extern "C" {
#endif

/*! \addtogroup plugin_parameters
 *  @{
 */
 
/*!
 \brief Create a parameter of type "type"
 \param label The parameters label as a string
 \param type  The parameters type as string. Possible values can be 'double', 'int', 'char*' etc,
 \param hint  The parameters hint as string. 
 \param value The parameters initial value casted to a (void*) pointer
 \return Returns a handle to a new parameter, if succesful, NULL otherwise
*/
RRP_DECLSPEC RRParameterHandle rrp_cc createParameter(const char* label, const char* type, const char* hint, void* value);

/*!
 \brief Free the memory created by a parameter
 \param paraHandle A handle to the parameter
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc freeParameter(RRParameterHandle para);


/*!
 \brief Add a parameter to a parameters container, from a parameter pointer.
 \param handle Handle to a RoadRunner instance
 \param para Handle to a roadrunner parameter
 \return Returns a booelan indicating success
*/
RRP_DECLSPEC bool rrp_cc addParameterToList(RRParametersHandle handle, RRParameterHandle para);

/*!
 \brief Set a parameter by a string
 \param handle to a Parameter instance
 \param value Pointer to string holding the value to assign to the parameter, e.g. "0.01" to set a double to 0.01
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setParameterByString(RRParameterHandle handle, const char* value);

/*!
 \brief Get a boolean parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc getBoolParameter(RRParameterHandle handle, bool* value);

/*!
 \brief Set a boolean parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setBoolParameter(RRParameterHandle handle, bool value);

/*!
 \brief Set an int parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setIntParameter(RRParameterHandle handle, int value);

/*!
 \brief Get the value of an int parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc getIntParameter(RRParameterHandle handle, int *value);

/*!
 \brief Set a double parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setDoubleParameter(RRParameterHandle handle, double value);

/*!
 \brief Get the value of a double parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc getDoubleParameter(RRParameterHandle handle, double *value);

/*!
 \brief Set a string (char*) parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setStringParameter(RRParameterHandle handle, char* value);

/*!
 \brief Get the value of a string (char*) parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc getStringParameter(RRParameterHandle handle, const char* (*value));

/*!
 \brief Set a listOfParameters (Parameters) parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setListParameter(RRParameterHandle handle, void* (value));

/*!
 \brief Get the value of a listOfParameters (Parameters) parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc getListParameter(RRParameterHandle handle, void* (value));


/*!
 \brief Set a roadRunnerDataParameter parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setRoadRunnerDataParameter(RRParameterHandle handle, void* value);

/*!
 \brief Get the value of a roadRunnerDataParameter parameter
 \param handle to a Parameter instance
 \param value to assign to the parameter.
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc getRoadRunnerDataParameter(RRParameterHandle handle, void* value);

/*!
 \brief Get a parameters info
 \param handle Handle to a parameter instance
 \return Returns informational text about the parameter if sucessful, NULL otherwise
*/
RRP_DECLSPEC char* rrp_cc getParameterInfo(RRParameterHandle handle);

/*!
 \brief Get a parameters value as char*
 \param handle to a Parameter instance
 \return Returns the parameters value if sucessful, NULL otherwise
*/
RRP_DECLSPEC char* rrp_cc getParameterValueAsString(RRParameterHandle handle);

/*!
 \brief Get a handle to a parameters value
 \param handle to a Parameter instance
 \return Returns a Handle to the parameters value if sucessful, NULL otherwise
*/
RRP_DECLSPEC void* rrp_cc getParameterValueHandle(RRParameterHandle handle);

/*!
 \brief Get a parameters name
 \param handle to a Parameter instance
 \return Returns the parameters name if sucessful, NULL otherwise
*/
RRP_DECLSPEC char* rrp_cc getParameterName(RRParameterHandle handle);

/*!
 \brief Get a parameters hint
 \param handle to a Parameter instance
 \return Returns the parameters hint if sucessful, NULL otherwise
*/
RRP_DECLSPEC char* rrp_cc getParameterHint(RRParameterHandle handle);

/*!
 \brief Set a parameters hint
 \param handle to a Parameter instance
 \param value The parameter hint as a string
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setParameterHint(RRParameterHandle handle, const char* value);

/*!
 \brief Get a parameters description
 \param handle to a Parameter instance
 \return Returns the parameters description as a string sucessful, NULL otherwise
*/
RRP_DECLSPEC char* rrp_cc getParameterDescription(RRParameterHandle handle);

/*!
 \brief Set a parameters Description
 \param handle to a Parameter instance
 \param value The parameter description as a string
 \return Returns true if sucessful, false otherwise
*/
RRP_DECLSPEC bool rrp_cc setParameterDescription(RRParameterHandle handle, const char* value);

/*!
 \brief Get a parameters type
 \param handle to a Parameter instance
 \return Returns the parameters type if sucessful, NULL otherwise
*/
RRP_DECLSPEC char* rrp_cc getParameterType(RRParameterHandle handle);


RRP_DECLSPEC RRParameterHandle rrp_cc getFirstParameter(RRParametersHandle handle);
RRP_DECLSPEC RRParameterHandle rrp_cc getNextParameter(RRParametersHandle handle);
RRP_DECLSPEC RRParameterHandle rrp_cc getPreviousParameter(RRParametersHandle handle);
RRP_DECLSPEC RRParameterHandle rrp_cc getCurrentParameter(RRParametersHandle handle);

/*!
 \brief Get a list of names for a plugins parameters.
 \param handle Handle to a plugin
 \return Returns a string with the names of each parameter, NULL otherwise
 \ingroup plugins
*/
RRP_DECLSPEC char* rrp_cc getNamesFromParameterList(RRParametersHandle handle);

/*!
 \brief Clear a list of parameters
 \param handle Handle to a Parameters list
  \return Returns true or false indicating result
 \ingroup plugins
*/
RRP_DECLSPEC bool rrp_cc clearParameterList(RRParametersHandle handle);

/*! @} */

#if defined(__cplusplus)
}}    //rrc namespace
#endif
  

#endif
