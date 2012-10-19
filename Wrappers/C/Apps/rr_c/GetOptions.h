#ifndef GetOptionsH
#define GetOptionsH                                                                                  \
#include "rrExporter.h"
//---------------------------------------------------------------------------

extern int                         optind;
extern int                         opterr;
extern char                       *optarg;
       int                         GetOptions(int argc, char *argv[], char *optstring);


#endif
