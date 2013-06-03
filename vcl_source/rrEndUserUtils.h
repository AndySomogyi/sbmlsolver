#ifndef rrEndUserUtilsH
#define rrEndUserUtilsH
#include "rrNewArrayList.h"
#include "rrc_types.h"

rr::NewArrayList 	convertCList(rrc::RRListHandle arrayList);
rr::StringList 		convertCStringArray(rrc::RRStringArrayHandle arrayList);

#endif
