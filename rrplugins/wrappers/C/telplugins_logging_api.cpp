#pragma hdrstop
#include "rrplugins/common/telLogger.h"
#include "telplugins_logging_api.h"
#include "telplugins_cpp_support.h"
//---------------------------------------------------------------------------

using namespace std;
using namespace tlp;
using namespace tlpc;

#include "telplugins_cpp_support.h"
#include "tel_macros.h"
#define start_try                                           \
        try                                                 \
        {

/*!
 \brief macro for catch statement in a routine returning a bool
 \ingroup macros
*/
#define  catch_bool_macro                                   \
    }                                                       \
    catch(exception& ex)                                    \
    {                                                       \
        stringstream msg;                                   \
        msg<<"Tellurium exception: "<<ex.what();            \
        tpSetError(msg.str());                                \
        return false;                                       \
    }

/*!
 \brief macro for catch statement in a routine returning a pointer
 \ingroup macros
*/
#define  catch_ptr_macro                                    \
    }                                                       \
    catch(exception& ex)                                    \
    {                                                       \
        stringstream msg;                                   \
        msg<<"Tellurium exception: "<<ex.what();            \
        tpSetError(msg.str());                                \
        return NULL;                                        \
    }

/*!
 \brief macro for catch statement in a routine returning a positive integer
 \ingroup macros
*/
#define catch_int_macro                                     \
    }                                                       \
    catch(exception& ex)                                    \
    {                                                       \
        stringstream msg;                                   \
        msg<<"Tellurium exception: "<<ex.what();            \
        tpSetError(msg.str());                                \
        return -1;                                          \
    }

/*!
 \brief macro for catch statement in a routine returning an unsigned integer
 \ingroup macros
*/
#define catch_uint_macro                                     \
    }                                                       \
    catch(exception& ex)                                    \
    {                                                       \
        stringstream msg;                                   \
        msg<<"Tellurium exception: "<<ex.what();            \
        tpSetError(msg.str());                                \
        return 0;                                          \
    }

/*!
 \brief macro for catch statement in a routine returning void
 \ingroup macros
*/
#define catch_void_macro                                    \
    }                                                       \
    catch(const exception& ex)                              \
    {                                                       \
        stringstream msg;                                   \
        msg<<"Tellurium exception: "<<ex.what();            \
        tpSetError(msg.str());                                \
    }




bool tlp_cc tpEnableLoggingToConsole()
{
    start_try
        Logger::enableConsoleLogging();
        return true;
    catch_bool_macro
}

bool tlp_cc tpDisableLoggingToConsole()
{
    start_try
        Logger::disableConsoleLogging();
        return true;
    catch_bool_macro
}

bool tlp_cc tpEnableLoggingToFile(char* fName)
{
    start_try
//        char* tempFolder = getTempFolder(handle);
        string logFile = string(fName);//joinPath(tempFolder, "RoadRunner.log") ;
//        tel::freeText(tempFolder);

        Logger::enableFileLogging(logFile);
        return true;
    catch_bool_macro
}

bool tlp_cc tpDisableLoggingToFile()
{
    start_try
        Logger::disableFileLogging();
        return true;
    catch_bool_macro
}

bool tlp_cc tpSetLogLevel(const char* _lvl)
{
    start_try
        Logger::Level lvl = Logger::stringToLevel(_lvl);
        tlp::Logger::setLevel(lvl);
        //rr::Logger::setLevel(lvl);
        return true;
    catch_bool_macro
}

char* tlp_cc tpGetLogLevel()
{
    start_try
        string level = Logger::getCurrentLevelAsString();
        char* lvl = tpCreateText(level.c_str());
        return lvl;
    catch_ptr_macro
}

char* tlp_cc tpGetLogFileName()
{
    start_try
        return tpCreateText(Logger::getFileName().c_str());
    catch_ptr_macro
}

void tlp_cc tpLogMsg(CLogLevel lvl, const char* msg)
{
    start_try
        if(msg)
        {
            RRPLOG((LogLevel) lvl)<<msg;
        }
        else
        {
            RRPLOG((LogLevel) lvl)<<"You passed a NULL message pointer to logMsg";
        }
    catch_void_macro
}
