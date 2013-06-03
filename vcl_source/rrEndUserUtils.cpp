#pragma hdrstop
#include "rrEndUserUtils.h"
#include "rrLogger.h"
#include "rrVCLUtils.h"
//---------------------------------------------------------------------------

using namespace rr;
using namespace rrc;
NewArrayList convertCList(RRListHandle inList)
{
	NewArrayList theList;

    if(!inList)
    {
    	return theList;
    }

	for(int i = 0; i < inList->Count; i++)
    {
    	RRListItemHandle item = inList->Items[i];
    	if(item->ItemType == litList)
        {
        	NewArrayList subList = convertCList(item->data.lValue);
            theList.Add(subList);
        }

        if(item->ItemType == litString)
        {
          	theList.Add(item->data.sValue);
        }
    }

	Log(lDebug) << "List :" << theList;
    return theList;
}

StringList convertCStringArray(RRStringArrayHandle inList)
{
	StringList theList;

    if(!inList)
    {
    	return theList;
    }

	for(int i = 0; i < inList->Count; i++)
    {

    	char* item = inList->String[i];
        if(item)
        {
          	theList.add(item);
        }
    }

	Log(lDebug) << "List :" << theList;
    return theList;
}

