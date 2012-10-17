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
char*       WinPathSeparator = "\\";
char*       LinuxPathSeparator = "/";

string		DefaultCompiler = "tcc";
char 	 	tab 			= '\t';
double    	DoubleNaN   = std::numeric_limits<double>::quiet_NaN() ;
float     	FloatNaN    = std::numeric_limits<float>::quiet_NaN() ;
//int       	MAX_MODULE  = 512;


}

