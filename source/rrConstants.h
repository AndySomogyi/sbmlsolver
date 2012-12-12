#ifndef rrConstantsH
#define rrConstantsH
#include <limits>
#include <string>
#include "rrExporter.h"
using namespace std;

namespace rr
{

RR_DECLSPEC extern const char 	PathSeparator;
RR_DECLSPEC extern const string	EmptyString;
RR_DECLSPEC extern const char* 	double_format;
RR_DECLSPEC extern const char* 	int_format;

RR_DECLSPEC extern const string gExeSuffix;
RR_DECLSPEC extern const char* 	Comma;
RR_DECLSPEC extern const string DefaultCompiler;

// Typedefs
typedef unsigned int 		u_int;
typedef long*   			IntPtr;

// Constants
RR_DECLSPEC extern const char 	tab;
RR_DECLSPEC extern const double	DoubleNaN;
RR_DECLSPEC extern const float  FloatNaN;

#ifndef MAXPATH
#define MAXPATH 250
#endif

const int MAX_MODULE = 512;

// Enums...
enum SBMLType {stCompartment = 0, stSpecies, stParameter};    //Species clashes with class Species, prefix enums with st, for SbmlType

}
#endif
