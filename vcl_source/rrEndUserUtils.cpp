#pragma hdrstop
#include "rrEndUserUtils.h"
#include "rrLogger.h"
#include "utils.h"
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

StringList GetCheckedItems(TCheckListBox* listBox)
{
    //Go trough the listbox and return checked items
    StringList checked;
    for(int i = 0; i < listBox->Count; i++)
    {
        if(listBox->Checked[i])
        {
            String anItem = listBox->Items->Strings[i];
            checked.Add(std_str(anItem));
        }
    }
    return checked;
}

