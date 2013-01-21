#pragma hdrstop
#include <sstream>
#include <iomanip>
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
{

}

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

    msg<<setfill('.');
    msg<<setw(30)<<left<<"Name of plugin"<<mName<<"\n";
    msg<<setw(30)<<left<<"Author"<<mAuthor<<"\n";
    msg<<setw(30)<<left<<"Category"<<mCategory<<"\n";
    msg<<setw(30)<<left<<"Version"<<mVersion<<"\n";
    msg<<setw(30)<<left<<"Copyright"<<mCopyright<<"\n";

    return msg.str();
}

}

