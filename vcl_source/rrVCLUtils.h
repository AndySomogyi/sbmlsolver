#ifndef rrVCLUtilsH
#define rrVCLUtilsH
#include <Vcl.CheckLst.hpp>
#include <sstream>
#include <string>
#include "rrStringlist.h"


using std::string;
using std::wstring;
//---------------------------------------------------------------------------
string                 	stdstr( const char* str );
string                 	stdstr( const string& str );
string                 	stdstr( const wstring& str );
string                 	stdstr( const String& str );

String                 	vclstr(const std::string& s);
String                 	vclstr(const char* s);
String                 	vclstr(const AnsiString& s);
String                 	vclstr(const std::ostringstream& s);

rr::StringList 	       	GetCheckedItems(TCheckListBox* listBox);
void 			   		AddItemsToListBox(const rr::StringList& items, TCheckListBox *lb);
#endif
