#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrConstants.h"

//---------------------------------------------------------------------------
namespace rr
{
const char*     	Comma 		= ",";
const string    	EmptyString 	= "";

const char* 		double_format = "%f";
const char* 		int_format  = "%d";;

#if defined(_WIN32) || defined(__CODEGEARC__)
const char       	PathSeparator = '\\';
const string		gExeSuffix	= ".exe";
#elif defined(__linux)
const char       	PathSeparator = '/';
const string		gExeSuffix	= "";
#else
const char       	PathSeparator = '/';
const string		gExeSuffix	= "";
#endif


const string		gDefaultSupportCodeFolder = "..\\rr_support";
const string		gDefaultCompiler = "..\\compilers\\tcc\\tcc.exe";
const char 	 		tab 			= '\t';
const double    	DoubleNaN   = std::numeric_limits<double>::quiet_NaN() ;
const float     	FloatNaN    = std::numeric_limits<float>::quiet_NaN() ;
//int       	MAX_MODULE  = 512;


}

