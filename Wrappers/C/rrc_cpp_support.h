/**
 * @file rrc_cpp_support.h
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

#ifndef rrc_cpp_supportH
#define rrc_cpp_supportH

#include <vector>
#include "rr-libstruct/lsMatrix.h"
#include "rrRoadRunnerData.h"
#include "rrRoadRunnerList.h"
#include "rrStringList.h"
#include "rrPlugin.h"
#include "rrNewArrayList.h"
#include "rrParameter.h"
#include "rrUtils.h"
#include "rrMinimizationData.h"
#include "rrc_types.h"
#include "rrc_exporter.h"

//When using the rrc_api from C++, the following routines are useful
namespace rrc
{
using std::vector;
void 									setError(const string& err);
//Cast void* handle to RoadRunner instance pointer, throw if it fails
rr::RoadRunner* 						castFrom(rrc::RRHandle rrHandle);

//Cast void* handle to RoadRunner instance pointer, throw if it fails
rr::Plugin* 							castToPlugin(rrc::RRPluginHandle handle);

//Cast void* handle to RoadRunner instance pointer, throw if it fails
rr::BaseParameter*						castToParameter(rrc::RRParameterHandle handle);

//Cast void* handle to MinimizationData instance pointer, throw if it fails
rr::MinimizationData*					castToMinimizationData(rrc::RRMinimizationDataHandle handle);

//Cast
rr::RoadRunnerList* 					getRRList(rrc::RRInstanceListHandle handle);
void                                    setError(const string& err);
bool                                    copyVector(const rrc::RRVector* source, vector<double>& dest);
rrc::RRVectorHandle                     createVectorFromVector_double(const vector<double>& vec);
vector<double>                          createVectorFromRRVector(const rrc::RRVector* vec);
rrc::RRMatrixHandle                     createMatrix(const ls::DoubleMatrix* mat);

//Lists and arrays
rrc::RRStringArrayHandle                createList(const rr::StringList& aList);
rrc::RRListHandle 		                createArrayList(const rr::NewArrayList& aList);

//Result data
C_DECL_SPEC rrc::RRDataHandle 			createRRData(const rr::RoadRunnerData& result);

}
#endif


