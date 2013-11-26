/**
 * @file rrc_minimization_api.h
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

#ifndef rrp_minimization_apiH
#define rrp_minimization_apiH
#include "rrp_exporter.h"
#include "rrp_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrp
{
extern "C"
{
#endif


/*!
 \brief Add parameter to fit to minimization data structure
 \param[in] handle Handle to a Minimization data structure
 \param[in] name Name of parameter
 \param[in] value value for the paramaeter
 \return Returns true if sucessful, false otherwise
 \ingroup Minimization
*/
//RRP_DECLSPEC bool rrp_cc addDoubleParameter(RRMinimizationDataHandle handle, const char* name, double value);

/*!
 \brief Set minimization objects selection list related to experimental data
 \param[in] handle Handle to a Minimization data structure
 \param[in] list Selectionlist as a string
 \return Returns true if sucessful, false otherwise
 \ingroup Minimization
*/
//RRP_DECLSPEC bool rrp_cc setMinimizationObservedDataSelectionList(RRMinimizationDataHandle handle, const char* list);

/*!
 \brief Set minimization objects selection list related to model data
 \param[in] handle Handle to a Minimization data structure
 \param[in] list Selectionlist as a string
 \return Returns true if sucessful, false otherwise
 \ingroup Minimization
*/
//RRP_DECLSPEC bool rrp_cc setMinimizationModelDataSelectionList(RRMinimizationDataHandle handle, const char* list);

/*!
 \brief print out some information about the MinimizationData
 \param[in] handle Handle to a Minimization data structure
 \return pointer to a string if succesful, NULL otherwise
 \ingroup Minimization
*/
RRP_DECLSPEC char* rrp_cc getMinimizationDataReport(RRMinimizationDataHandle handle);

#if defined(__cplusplus)
}    //Extern "C"

}    //rrc namespace
#endif

#endif

