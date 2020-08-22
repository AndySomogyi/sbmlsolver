#pragma once

// API used to connect roadrunner functionality with plugins

#include "../../../wrappers/C/rrc_types.h"		//decleration of types such as rrhandle

#if defined(__cplusplus)
	namespace rrc { 
	extern "C" {
#endif

typedef struct
{
	const char* (*pluginName)();	 
} TPluginInterface;


typedef struct {			// THostInterface

	/*!
	 \brief Initialize a new roadRunner instance and return a handle to it.
	 \return Returns a RoadRunner instance, returns null if it fails
	 \ingroup initialization
	*/
	RRHandle  (*createRRInstance)(void);

	/*!
	 \brief Retrieve info about current state of roadrunner, e.g. loaded model, conservationAnalysis etc.
	 \param[in] handle Handle to a RoadRunner instance
	 \return Returns null if it fails, otherwise it returns a string with the info
	 \ingroup utility
	*/
	char*   (*getInfo)(RRHandle handle);

	/*!
	\brief Load a model from an SBML string
	\param[in] handle Handle to a RoadRunner instance
	\param[in] sbml string
	\return Returns true if successful
	\ingroup loadsave
	*/
	bool  (*loadSBML)(RRHandle handle, const char* sbml);

	/*!
	\brief Carry out a time-course simulation based on the given arguments, time start,
	time end and number of points.

	\param[in] handle Handle to a RoadRunner instance
	\param[in] timeStart Time start
	\param[in] timeEnd Time end
	\param[in] numberOfPoints Number of points to generate
	\return Returns an array (RRCDataPtr) of columns containing the results of the
	simulation including string labels for the individual columns. The client is
	responsible for freeing the resulting RRCDataPtr structure.
	\ingroup simulation
	*/

	RRCDataPtr (*simulateEx)(RRHandle handle, const double timeStart, const double timeEnd, const int numberOfPoints);


} THostInterface;

//THostInterface hostInterfaceMain;

#if defined( __cplusplus)
		}//extern
	}//namespace
#endif

