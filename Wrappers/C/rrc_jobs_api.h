/**
 * @file rrc_jobs_api.h
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

#ifndef rrc_jobs_apiH
#define rrc_jobs_apiH
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
 \brief Typedef for callback function, taking a void* parameter
 \ingroup multithreading
*/
typedef void    (rrcCallConv *callBackFunc)(void*);

/*!
 \brief Run a roadrunner simulation in a thread
 \param[in] handle Handle to a RoadRunner instance
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobHandle 	rrcCallConv simulateJob(RRHandle handle);

/*!
 \brief Run a roadrunner simulation in a thread
 \param[in] handle Handle to a RoadRunner instance
 \param timeStart,timeEnd,numberOfPoints Parameters for the simulation
 \param fn1,fn2 Callback functions that are used internally in the thread. 
 \param userData User supplied data that can be used in the supplied callback functions
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobHandle 	rrcCallConv simulateJobEx(RRHandle handle, double timeStart,
														double timeEnd,	int numberOfPoints,
                                                        callBackFunc fn1, callBackFunc fn2, void* userData);

/*!
 \brief Run multiple simulations in a set of threads
 \param[in] handles Handle to a list of RoadRunner instances
 \param[in] nrOfThreads Number of threads to allocate in order to execute the jobs
 \return Returns a handle to a list of jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle 	rrcCallConv simulateJobs(RRInstanceListHandle handles, int nrOfThreads);


/*!
 \brief Run multiple simulations in a set of threads
 \param[in] handles Handle to a list of RoadRunner instances
 \param[in] nrOfThreads Number of threads to allocate in order to execute the jobs
 \param timeStart,timeEnd,numberOfPoints Parameters for the simulation
 \param fn1,fn2 Callback functions that are used internally in the thread. 
 \param userData User supplied data that can be used in the supplied callback functions
 \return Returns a handle to a list of jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/C_DECL_SPEC RRJobsHandle 	rrcCallConv simulateJobsEx(RRInstanceListHandle handles, int nrOfThreads, 
                                                        double timeStart, double timeEnd, int numberOfPoints,
                                                        callBackFunc fn1, callBackFunc fn2, void* userData);

/*!
 \brief Load a model from a SBML file into a RoadRunner instances, using a Job
 \param[in] handle Handle to a RoadRunner instance
 \param[in] fileName file name (with optional full path) to a file that holds the SBML model
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/

C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLFromFileJob(RRHandle handle, const char* fileName);

/*!
 \brief Load a model from a SBML file into a RoadRunner instances, using a Job
 \param[in] handle Handle to a RoadRunner instance
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \param[in] reCompile Boolean that forces recompilation if true. If false, no compilation occur if model dll exists
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/

C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLFromFileJobEx(RRHandle handle, const char* fileName, bool reCompile);

/*!
 \brief Load a model from a SBML file into a set of RoadRunner instances
 \param[in] handles Handle to a list of RoadRunner instances
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \param[in] nrOfThreads Number of threads to allocate in order to execute the job
 \return Returns a handle to the Jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle rrcCallConv loadSBMLFromFileJobs(RRInstanceListHandle handles, const char* fileName, int nrOfThreads);

/*!
 \brief Load a model from a SBML text string into a RoadRunner instances, using a Job
 \param[in] handle Handle to a RoadRunner instance
 \param[in] sbml String that holds the SBML model
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLJob(RRHandle handle, const char* sbml);

/*!
 \brief Load a model from a SBML text string into a RoadRunner instances, using a Job
 \param[in] handle Handle to a RoadRunner instance
 \param[in] sbml String that holds the SBML model
 \param[in] reCompile Boolean that forces recompilation if true. If false, no compilation occur if model dll exists
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLJobEx(RRHandle handle, const char* sbml, bool reCompile);

/*!
 \brief Load a model from a SBML tesxt string into a set of RoadRunner instances
 \param[in] handles Handle to a list of RoadRunner instances
 \param[in] sbml String that holds the SBML model
 \param[in] nrOfThreads Number of threads to allocate in order to execute the job
 \return Returns a handle to the Jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle rrcCallConv loadSBMLJobs(RRInstanceListHandle handles, const char* sbml, int nrOfThreads);

/*!
 \brief Load a model from a SBML file into a set of RoadRunner instances
 \param[in] handles Handle to a list of RoadRunner instances
 \param[in] sbml string that holds the SBML model
 \param[in] nrOfThreads Number of threads to allocate in order to execute the job
 \param[in] force_recompile Indicates if a forced recompilation of the model should be executed
 \return Returns a handle to the Jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle rrcCallConv loadSBMLJobsEx(RRInstanceListHandle handles, const char* sbml, int nrOfThreads, bool force_recompile);

/*!
 \brief Wait for jobs in thread to finish
 \param[in] handle Handle to a RoadRunner job
 \return Returns true if thread finsihed up properly, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv waitForJob(RRJobHandle handle);

/*!
 \brief Wait for jobs in thread pool to finish
 \param[in] handle Handle to a list of RoadRunner jobs
 \return Returns true if threadpool finished up properly, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv waitForJobs(RRJobsHandle handle);

/*!
 \brief Check if there are work being done on a job
 \param[in] handle Handle to a RoadRunner job
 \return Returns true if there are running threads, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv isJobFinished(RRJobHandle handle);

/*!
 \brief Check if there are work being done on jobs
 \param[in] handle Handle to a list of RoadRunner jobs
 \return Returns true if there are running threads, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv areJobsFinished(RRJobsHandle handle);

/*!
 \brief Get number of remaining jobs in a job list
 \param[in] handle Handle to a list of RoadRunner jobs
 \return Returns number of remaining, unfinished jobs. Returns -1 on failure
 \ingroup multiThreading
*/
C_DECL_SPEC int rrcCallConv getNumberOfRemainingJobs(RRJobsHandle handle);


/*!
 \brief Free a job handle
 \param[in] handle Handle to a roadrunner job
 \return Returns true or false indicating if the function was succesful
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv freeJob(RRJobHandle handle);

/*!
 \brief Free a jobs handle
 \param[in] handle Handle to a list of roadrunner jobs
 \return Returns true or false indicating if the function was succesful
 \ingroup multiThreading
*/

C_DECL_SPEC bool rrcCallConv freeJobs(RRJobsHandle handle);

#if defined(__cplusplus)
}	//Extern "C"
}	//namespace
#endif

#endif
