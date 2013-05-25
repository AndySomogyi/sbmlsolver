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

C_DECL_SPEC RRJobHandle rrCallConv simulateJob(RRHandle rrHandle);
C_DECL_SPEC RRJobsHandle rrCallConv simulateJobs(RRInstanceListHandle rrHandles, int nrOfThreads);


/*!
 \brief Load a model from a SBML file into a RoadRunner instances, using a Job
 \param[in] rrHandle - RoadRunner handle
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/

C_DECL_SPEC RRJobHandle rrCallConv loadSBMLFromFileJob(RRHandle rrHandle, const char* fileName);

/*!
 \brief Load a model from a SBML file into a RoadRunner instances, using a Job
 \param[in] rrHandle - RoadRunner handle
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \param[in] recompileFlag, Boolean that forces recompilation if true. If false, no compilation occur if model dll exists
 \return Returns a handle to the Job if succesful, otherwise returns NULL
 \ingroup multiThreading
*/

C_DECL_SPEC RRJobHandle rrCallConv loadSBMLFromFileJobEx(RRHandle rrHandle, const char* fileName, bool reCompile);

/*!
 \brief Free a job handle
 \param[in] RRJobHandle
 \return Returns true or false indicating if the function was succesful
 \ingroup multiThreading
*/

C_DECL_SPEC bool rrCallConv freeJob(RRJobHandle jobHandle);

/*!
 \brief Load a model from a SBML file into a set of RoadRunner instances
 \param[in] rrHandles - RoadRunner handles structure
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \return Returns a handle to the Jobs if succesful, otherwise returns NULL
 \ingroup multiThreading
*/
C_DECL_SPEC RRJobsHandle rrCallConv loadSBMLFromFileJobs(RRInstanceListHandle rrHandles, const char* fileName, int nrOfThreads);

/*!
 \brief Wait for jobs in thread to finish
 \param[in] RRJobHandle - aHandle to a roadrunner thread
 \return Returns true if thread finsihed up properly, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrCallConv waitForJob(RRJobHandle handle);

/*!
 \brief Wait for jobs in thread pool to finish
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns true if threadpool finished up properly, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrCallConv waitForJobs(RRJobsHandle handle);

/*!
 \brief Check if there are work being done on a job
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns true if there are running threads, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrCallConv isJobFinished(RRJobHandle handle);

/*!
 \brief Check if there are work being done on jobs
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns true if there are running threads, otherwise returns false
 \ingroup multiThreading
*/
C_DECL_SPEC bool rrCallConv areJobsFinished(RRJobsHandle handle);

/*!
 \brief Get number of remaining jobs in a threadPool
 \param[in] RRJobsHandle - aHandle to a threadPool
 \return Returns number of remaining, unfinished jobs. Returns -1 on failure
 \ingroup multiThreading
*/
C_DECL_SPEC int rrCallConv getNumberOfRemainingJobs(RRJobsHandle handle);


#if defined(__cplusplus)
}	//Extern "C"
}	//namespace
#endif

#endif
