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

typedef void    (rrcCallConv *callBackFunc)(void*);

C_DECL_SPEC RRJobHandle 	rrcCallConv simulateJob(RRHandle rrHandle);
C_DECL_SPEC RRJobHandle 	rrcCallConv simulateJobEx(RRHandle rrHandle, double timeStart,
														double timeEnd,	int numberOfPoints,
                                                        callBackFunc fn1, callBackFunc fn2, void* userData);

C_DECL_SPEC RRJobsHandle 	rrcCallConv simulateJobs(RRInstanceListHandle rrHandles, int nrOfThreads);
C_DECL_SPEC RRJobsHandle 	rrcCallConv simulateJobsEx(RRInstanceListHandle rrHandles,
														int nrOfThreads, double timeStart,
                                                        double timeEnd, int numberOfPoints,
                                                        callBackFunc fn1, callBackFunc fn2, void* userData);

/*!
 \brief Load a model from a SBML file into a RoadRunner instances, using a Job
 \param[in] rrHandle - RoadRunner handle
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/

C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLFromFileJob(RRHandle rrHandle, const char* fileName);

/*!
 \brief Load a model from a SBML file into a RoadRunner instances, using a Job
 \param[in] rrHandle - RoadRunner handle
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \param[in] recompileFlag, Boolean that forces recompilation if true. If false, no compilation occur if model dll exists
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/

C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLFromFileJobEx(RRHandle rrHandle, const char* fileName, bool reCompile);

/*!
 \brief Load a model from a SBML file into a set of RoadRunner instances
 \param[in] rrHandles - RoadRunner handles structure
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \return Returns a handle to the Jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle rrcCallConv loadSBMLFromFileJobs(RRInstanceListHandle rrHandles, const char* fileName, int nrOfThreads);

/*!
 \brief Load a model from a SBML text string into a RoadRunner instances, using a Job
 \param[in] rrHandle - RoadRunner handle
 \param[in] sbml string that holds the SBML model
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLJob(RRHandle rrHandle, const char* sbml);

/*!
 \brief Load a model from a SBML text string into a RoadRunner instances, using a Job
 \param[in] rrHandle - RoadRunner handle
 \param[in] sbml string that holds the SBML model
 \param[in] recompileFlag, Boolean that forces recompilation if true. If false, no compilation occur if model dll exists
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobHandle rrcCallConv loadSBMLJobEx(RRHandle rrHandle, const char* sbml, bool reCompile);

/*!
 \brief Load a model from a SBML tesxt string into a set of RoadRunner instances
 \param[in] rrHandles - RoadRunner handles structure
 \param[in] sbml string that holds the SBML model
 \return Returns a handle to the Jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle rrcCallConv loadSBMLJobs(RRInstanceListHandle rrHandles, const char* sbml, int nrOfThreads);

/*!
 \brief Load a model from a SBML file into a set of RoadRunner instances
 \param[in] rrHandles - RoadRunner handles structure
 \param[in] sbml string that holds the SBML model
 \return Returns a handle to the Jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle rrcCallConv loadSBMLJobsEx(RRInstanceListHandle rrHandles, const char* sbml, int nrOfThreads, bool recompile);

/*!
 \brief Wait for jobs in thread to finish
 \param[in] RRJobHandle - aHandle to a roadrunner thread
 \return Returns true if thread finsihed up properly, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv waitForJob(RRJobHandle handle);

/*!
 \brief Wait for jobs in thread pool to finish
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns true if threadpool finished up properly, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv waitForJobs(RRJobsHandle handle);

/*!
 \brief Check if there are work being done on a job
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns true if there are running threads, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv isJobFinished(RRJobHandle handle);

/*!
 \brief Check if there are work being done on jobs
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns true if there are running threads, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrcCallConv areJobsFinished(RRJobsHandle handle);

/*!
 \brief Get number of remaining jobs in a threadPool
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns number of remaining, unfinished jobs. Returns -1 on failure
 \ingroup multiThreading
*/
C_DECL_SPEC int rrcCallConv getNumberOfRemainingJobs(RRJobsHandle handle);


/*!
 \brief Free a job handle
 \param[in] RRJobHandle
 \return Returns true or false indicating if the function was succesful
 \ingroup multiThreading
*/

C_DECL_SPEC bool rrcCallConv freeJob(RRJobHandle handle);


/*!
 \brief Free a jobs handle
 \param[in] RRJobsHandle
 \return Returns true or false indicating if the function was succesful
 \ingroup multiThreading
*/

C_DECL_SPEC bool rrcCallConv freeJobs(RRJobsHandle handle);

#if defined(__cplusplus)
}	//Extern "C"
}	//namespace
#endif

#endif
