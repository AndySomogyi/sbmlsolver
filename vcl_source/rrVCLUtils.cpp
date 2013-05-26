#pragma hdrstop
#include "rrVCLUtils.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
namespace rr
{
using namespace std;
using namespace rr;


//Passing a NULL string, returns a empty std string..
string stdstr(const char* str)
{
	return (str != NULL) ? string(str) : string("");
}

string stdstr(const string& str)
{
	return str;
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

StringList getCheckedItems(TCheckListBox* listBox)
{
    //Go trough the listbox and return checked items
    StringList checked;
    for(int i = 0; i < listBox->Count; i++)
    {
        if(listBox->Checked[i])
        {
            String anItem = listBox->Items->Strings[i];
            checked.add(stdstr(anItem));
        }
    }
    return checked;
}

void addItemsToListBox(const rr::StringList& items, TCheckListBox *lb, bool checked)
{
    for(int i = 0; i < items.Count(); i++)
    {
        int index = lb->Items->Add(items[i].c_str());
        lb->Checked[index] = checked;
    }
}

void addItemsToListBox(const rr::StringList& items, TListBox *lb)
{
    for(int i = 0; i < items.Count(); i++)
    {
        int index = lb->Items->Add(items[i].c_str());
    }
}

int populateDropDown(set<string>& files, TComboBox *CB)
{
	//Populate the drop down.
	CB->Clear();
    set<string>::iterator it = files.begin();
    for (; it != files.end(); ++it)
    {
    	string file  = getFileNameNoExtension(*it);
		CB->Items->Add(file.c_str());
    }
	return files.size();
}

}
