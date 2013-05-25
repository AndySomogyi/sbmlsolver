#ifndef rrEndUserUtilsH
#define rrEndUserUtilsH
#include <Vcl.CheckLst.hpp>

#include "rrNewArrayList.h"
#include "rrc_types.h"


rr::NewArrayList convertCList(rrc::RRListHandle arrayList);

rr::StringList 	GetCheckedItems(TCheckListBox* listBox);
#endif
