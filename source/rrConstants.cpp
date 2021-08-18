#pragma hdrstop
#include "rrStringUtils.h"
#include "rrConstants.h"

//---------------------------------------------------------------------------
namespace rr
{

//Useful constants..
const char*         gComma                         = ",";
const char              gTab                         = '\t';
const char              gNL                         = '\n';
const char*         gDoubleFormat                 = "%f";
const char*         gIntFormat                  = "%d";;

const std::string        gNoneString                 = "<none>";
const std::string        gEmptyString                 = "";

const int             gMaxPath                    = 512;
const double        gDoubleNaN                   = std::numeric_limits<double>::quiet_NaN() ;
const float         gFloatNaN                    = std::numeric_limits<float>::quiet_NaN() ;

//Messages
const char*        gEmptyModelMessage             = "A model needs to be loaded before one can use this method";


#if defined(_WIN32) || defined(__CODEGEARC__)
    const char           gPathSeparator      = '\\';
    const std::string        gExeSuffix          = ".exe";
#elif defined(__unix__) || defined(__APPLE__)
    const char           gPathSeparator      = '/';
    const std::string        gExeSuffix          = "";
#else  //Something else...
    const char           gPathSeparator      = '/';
    const std::string        gExeSuffix          = "";
#endif

}

