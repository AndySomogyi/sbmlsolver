#ifndef rrGetOptionsH
#define rrGetOptionsH
#include "rrExporter.h"
//---------------------------------------------------------------------------
namespace rr
{
RR_DECLSPEC     extern  int                         optind;
RR_DECLSPEC     extern  int                         opterr;
RR_DECLSPEC     extern  char                       *optarg;
RR_DECLSPEC             int   GetOptions(int argc, char *argv[], char *optstring);
}

#endif
