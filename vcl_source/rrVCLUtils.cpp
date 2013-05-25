#pragma hdrstop

#include "rrVCLUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;
using namespace rr;
string stdstr(const string& str)
{
	return str;
}

string stdstr(const char* str)
{
	return string(str);
}

string stdstr(const String& str)
{
	return stdstr(wstring(str.c_str()));
}

string stdstr( const std::wstring& str )
{
	ostringstream stm;
	const ctype<char>& ctfacet =
						 use_facet< ctype<char> >( stm.getloc() );
	for( size_t i=0 ; i<str.size() ; ++i )
    {
		stm << ctfacet.narrow( str[i], 0 );
    }
	return stm.str() ;
}

String vclstr(const std::string& s)
{
	return String( s.c_str(), s.length() );
}

String vclstr(const std::ostringstream& s)
{
	return vclstr(s.str());
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
            checked.Add(stdstr(anItem));
        }
    }
    return checked;
}
void AddItemsToListBox(const rr::StringList& items, TCheckListBox *lb)
{
    for(int i = 0; i < items.Count(); i++)
    {
        int index = lb->Items->Add(items[i].c_str());
        lb->Checked[index] = true;
    }
}

