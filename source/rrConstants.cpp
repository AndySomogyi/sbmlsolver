#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrConstants.h"

//---------------------------------------------------------------------------
namespace rr
{
char*       Comma = ",";
char*       EmptyString = "";

#if defined(_WIN32) || defined(__CODEGEARC__)
char*       PathSeparator = "\\";
string		gExeSuffix	= ".exe";
#elif defined(__linux)
char*       PathSeparator = "/";
string		gExeSuffix	= "";
#else
char*       PathSeparator = "/";
string		gExeSuffix	= "";
#endif



string		DefaultCompiler = "tcc";
char 	 	tab 			= '\t';
double    	DoubleNaN   = std::numeric_limits<double>::quiet_NaN() ;
float     	FloatNaN    = std::numeric_limits<float>::quiet_NaN() ;
//int       	MAX_MODULE  = 512;


}

