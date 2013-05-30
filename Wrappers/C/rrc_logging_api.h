#ifndef rrc_logging_apiH
#define rrc_logging_apiH
#include "rrc_exporter.h"
#include "rrc_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrc
{
extern "C"
{
#endif

enum  CLogLevel
    {
        clShowAlways = -1,
        clError      = 0,
        clWarning    = 1,
        clInfo       = 2,
        clDebug      = 3,
        clDebug1     = 4,
        clDebug2     = 5,
        clDebug3     = 6,
        clDebug4     = 7,
        clDebug5     = 8,
        clAny        = 9,
        clUser
    };

/*!
 \brief Enable logging to console
 \return Returns true if succesful, false otherwise
 \ingroup logging
*/
C_DECL_SPEC bool rrcCallConv enableLoggingToConsole(void);

/*!
 \brief Enable logging to logFile
 \return Returns true if succesful, false otherwise
 \ingroup logging
*/

C_DECL_SPEC bool rrcCallConv enableLoggingToFile(RRHandle handle);


/*!
 \brief Set the logging status level

 The logging level is determined by the following strings

 "ANY", "DEBUG5", "DEBUG4", "DEBUG3", "DEBUG2", "DEBUG1",
 "DEBUG", "INFO", "WARNING", "ERROR"

 Example: \code setLogLevel ("DEBUG4") \endcode

 \param lvl Pointer to the logging level string.
 \return Ruturns true if succesful
 \ingroup logging
*/
C_DECL_SPEC bool rrcCallConv setLogLevel(const char* lvl);

/*!
 \brief Get the logging status level as a pointer to a string

 The logging level can be one of the following strings

 "ANY", "DEBUG5", "DEBUG4", "DEBUG3", "DEBUG2", "DEBUG1",
 "DEBUG", "INFO", "WARNING", "ERROR"

 Example: \code str = getLogLevel (void) \endcode

 \return Returns null if it fails else returns a pointer to the logging string
 \ingroup logging
*/
C_DECL_SPEC char* rrcCallConv getLogLevel(void);

/*!
 \brief Get a pointer to the string that holds the logging file name path

  Example: str = getLogFileName (void)

 \return Returns null if it fails otherwise returns the full path to the logging file name
 \ingroup logging
*/
C_DECL_SPEC char* rrcCallConv getLogFileName(void);


C_DECL_SPEC void rrcCallConv logMsg(enum CLogLevel lvl, const char* msg);

C_DECL_SPEC char* rrcCallConv testString (char* testStr);


#if defined(__cplusplus)
}	//Extern "C"

}	//rrc namespace
#endif

#endif
