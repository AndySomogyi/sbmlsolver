#ifndef rrVCLUtilsH
#define rrVCLUtilsH
#include <Vcl.CheckLst.hpp>
#include <sstream>
#include <string>
#include <set>
#include "rrStringlist.h"

namespace rr
{
using std::string;
using std::wstring;
using std::set;
//---------------------------------------------------------------------------
string                 	stdstr( const char* str );
string                 	stdstr( const string& str );
string                 	stdstr( const wstring& str );
string                 	stdstr( const String& str );

String                 	vclstr(const std::string& s);
String                 	vclstr(const char* s);
String                 	vclstr(const AnsiString& s);
String                 	vclstr(const std::ostringstream& s);

rr::StringList 	       	getCheckedItems(TCheckListBox* listBox);
void 			   		addItemsToListBox(const rr::StringList& items, TCheckListBox *lb, bool checked = false);
void 			   		addItemsToListBox(const rr::StringList& items, TListBox *lb);
int 					populateDropDown(set<string>& files, TComboBox *CB);
}

#endif
