//---------------------------------------------------------------------------
#pragma hdrstop
#include <sstream>
#include "rrPlugin.h"
//---------------------------------------------------------------------------
using namespace std;
namespace rr
{

Plugin::Plugin(const std::string& name, const std::string& cat)
:
mName(name),
mAuthor("Totte Karlsson"), 
mCategory(cat),
mVersion("0.1"), 
mCopyright("Totte Karlsson, Herbert Sauro, Systems Biology, UW 2012")
{}

Plugin::~Plugin()
{}

string Plugin::GetName()
{
	return mName;
}

string Plugin::GetAuthor()
{
	return mAuthor;
}

string Plugin::GetCategory()
{
	return mCategory;
}

string Plugin::GetVersion()
{
	return mVersion;
}

string Plugin::GetCopyright()
{
	return mCopyright;
}

string Plugin::GetInfo()
{
    stringstream msg;
    msg<<"Name of plugin: "<<mName<<"\n";
    msg<<"Authot: "<<mAuthor<<"\n";
    msg<<"Category: "<<mCategory<<"\n";
    msg<<"Version: "<<mVersion<<"\n";
    msg<<"Copyright: "<<mCopyright<<"\n";

    return msg.str();
}

}

