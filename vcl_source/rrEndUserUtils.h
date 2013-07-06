#ifndef rrEndUserUtilsH
#define rrEndUserUtilsH
#include "rrNewArrayList.h"
#include "rrc_types.h"

rr::NewArrayList 	convertCList(rrc::RRList* arrayList);
rr::StringList 		convertCStringArray(rrc::RRStringArray* arrayList);

#endif
